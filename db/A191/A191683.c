/*
 * A191683  Smallest representative squarefree composite n with prescribed
 *          number of prime factors and prescribed, prime arithmetic average
 *          of these factors.
 * https://oeis.org/A191683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A191683
 * The code computes the n-th smallest squarefree composite number
 * whose distinct prime factors have a prime arithmetic average.
 * It scans all composites up to a fixed bound, keeps the first
 * encounter for each (k, p) pair (k = number of distinct primes,
 * p = prime average), and stores those values in ascending order.
 * The array is then used to answer any a(n) query.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SP 10000000U           // maximal number to be sieved
#define TARGET_TERMS 30000         // precompute this many terms
#define HASH_SIZE (1 << 20)        // size of hash table (power of two)
#define HASH_MASK (HASH_SIZE - 1)

/* forward declarations for helpers */
static inline unsigned hash64(uint64_t key);
static inline bool pair_has(uint64_t key, uint64_t *table);
static inline void pair_put(uint64_t key, uint64_t *table);

/* main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static uint32_t *spf = NULL;           // smallest prime factor table
    static uint64_t *pair_table = NULL;    // hash table for (k,p) keys

    if (!initialized) {
        /* allocate and compute SPF up to MAX_SP */
        spf = (uint32_t *)calloc(MAX_SP + 1, sizeof(uint32_t));
        if (!spf) abort(); /* allocation failure */
        spf[0] = spf[1] = 1;
        for (uint32_t i = 2; i <= MAX_SP; ++i) {
            if (!spf[i]) {
                spf[i] = i;
                if ((uint64_t)i * i <= MAX_SP) {
                    for (uint32_t j = i * i; j <= MAX_SP; j += i)
                        if (!spf[j]) spf[j] = i;
                }
            }
        }

        /* allocate hash table for (k,p) pairs */
        pair_table = (uint64_t *)calloc(HASH_SIZE, sizeof(uint64_t));
        if (!pair_table) abort();

        /* allocate sequence array */
        seq_len = 0;
        size_t seq_cap = 1024;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();

        /* scan composites and build the sequence */
        for (uint32_t num = 4; num <= MAX_SP && seq_len < TARGET_TERMS; ++num) {
            if (spf[num] == num) continue;  // prime, skip

            uint32_t temp = num;
            uint32_t lastp = 0;
            uint32_t sum = 0;
            uint32_t kcnt = 0;
            bool squarefree = true;

            while (temp > 1) {
                uint32_t p = spf[temp];
                if (p == lastp) { squarefree = false; break; }
                lastp = p;
                sum += p;
                ++kcnt;
                while (temp % p == 0) temp /= p;
            }
            if (!squarefree || kcnt < 2) continue;

            if (sum % kcnt != 0) continue;
            uint32_t avg = sum / kcnt;
            if (spf[avg] != avg) continue;   // avg must be prime

            uint64_t key = ((uint64_t)avg << 32) | (uint64_t)kcnt;
            if (!pair_has(key, pair_table)) {
                pair_put(key, pair_table);
                /* store the value */
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!seq) abort();
                }
                seq[seq_len++] = (seq_t)num;
            }
        }

        initialized = true;
    }

    if (n == 0 || n > seq_len) return (seq_t)0;  /* out of bounds */
    return seq[n - 1];
}

/* ------------------------------------------------------------------ */
/* Hash table helpers (open addressing, linear probing) */
/* ------------------------------------------------------------------ */
static inline unsigned hash64(uint64_t key)
{
    /* a simple multiplicative hash */
    return (unsigned)((key * 1199540899ULL) & HASH_MASK);
}

static inline bool pair_has(uint64_t key, uint64_t *table)
{
    unsigned h = hash64(key);
    while (table[h]) {
        if (table[h] == key) return true;
        h = (h + 1) & HASH_MASK;
    }
    return false;
}

static inline void pair_put(uint64_t key, uint64_t *table)
{
    unsigned h = hash64(key);
    while (table[h]) {
        if (table[h] == key) return;
        h = (h + 1) & HASH_MASK;
    }
    table[h] = key;
}
