/*
 * A093451  Number of distinct prime divisors of
 *          Product_{k=1+(n-1)n/2..n(n+1)/2} k (i.e., of 1, 2*3, 4*5*6,
 *          7*8*9*10, ...).
 * https://oeis.org/A093451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime divisors of the product
 *            ∏_{k=(n−1)n/2+1}^{n(n+1)/2} k (block of n consecutive integers)
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   clear_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/clear_bit.h"

/* Adjust these constants if the maximum n is known to be larger. */
#define MAX_N 10000
#define LIMIT (MAX_N * (MAX_N + 1) / 2)   /* maximum value of R = n(n+1)/2 */

/* Global arrays used to avoid recomputation on successive calls. */
static uint32_t *spf          = NULL;   /* smallest prime factor for each number */
static uint32_t *primes       = NULL;   /* list of primes up to LIMIT */
static size_t    prime_count  = 0;
static size_t    prime_cap    = 0;

static uint8_t  *bitset        = NULL;   /* bitset for primes seen in current block */
static size_t    bitset_len    = 0;       /* number of bytes */

static seq_t    *memo         = NULL;   /* cached results for 1..MAX_N */
static bool      memo_done[MAX_N + 1] = {0};

static uint32_t *block_primes = NULL;   /* primes encountered in current block */
static size_t    block_cap    = 0;
static size_t    block_len    = 0;

/* Simple bitset helpers */
static inline void set_bit(uint8_t *b, uint32_t p) {
    b[p >> 3] |= (1u << (p & 7));
}
static inline bool test_bit(const uint8_t *b, uint32_t p) {
    return b[p >> 3] & (1u << (p & 7));
}

/* Two‑phase initialization: sieve + allocation of helper tables. */
static void init(void) {
    if (spf != NULL) return;   /* already initialized */

    /* Allocate and initialize spf array */
    spf = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(1);

    /* Dynamic array for primes */
    prime_cap = 32768;
    primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!primes) exit(1);

    /* Linear sieve */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                primes = (uint32_t*)realloc(primes,
                                            prime_cap * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[prime_count++] = i;
        }
        for (size_t j = 0; j < prime_count; ++j) {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)i * p;
            if (v > LIMIT) break;
            spf[(uint32_t)v] = p;
            if (p == spf[i]) break;
        }
    }

    /* Bitset to record primes seen in the current block */
    bitset_len = (LIMIT >> 3) + 1;
    bitset = (uint8_t*)calloc(bitset_len, sizeof(uint8_t));
    if (!bitset) exit(1);

    /* Buffer for primes encountered in a block */
    block_cap = 1024;
    block_primes = (uint32_t*)malloc(block_cap * sizeof(uint32_t));
    if (!block_primes) exit(1);

    /* Memoisation table for a(n) */
    memo = (seq_t*)malloc((MAX_N + 1) * sizeof(seq_t));
    if (!memo) exit(1);
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0 || n > MAX_N) return 0;   /* out of allowed range */

    init();   /* ensure tables are built once */

    if (memo_done[(size_t)n]) return memo[(size_t)n];

    /* Compute block bounds as 64‑bit to avoid overflow, then cast to 32‑bit. */
    uint64_t nn = (uint64_t)n;
    uint32_t L = (uint32_t)(nn * (nn - 1) / 2 + 1);
    uint32_t R = (uint32_t)(nn * (nn + 1) / 2);

    uint32_t count = 0;

    /* Process every number in the block. */
    for (uint32_t k = L; k <= R; ++k) {
        uint32_t x = k;
        while (x > 1) {
            uint32_t p = spf[x];
            if (!test_bit(bitset, p)) {
                set_bit(bitset, p);
                count++;
                if (block_len == block_cap) {
                    block_cap <<= 1;
                    block_primes = (uint32_t*)realloc(block_primes,
                                                       block_cap * sizeof(uint32_t));
                    if (!block_primes) exit(1);
                }
                block_primes[block_len++] = p;
            }
            while (x % p == 0) x /= p;
        }
    }

    /* Store result and mark as computed. */
    memo[(size_t)n] = (seq_t)count;
    memo_done[(size_t)n] = true;

    /* Clear the bitset for primes seen in this block so next call starts clean. */
    for (size_t i = 0; i < block_len; ++i)
        clear_bit(bitset, block_primes[i]);

    block_len = 0;

    return memo[(size_t)n];
}
