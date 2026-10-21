/*
 * A068362  Numbers n such that there are exactly 4 primes p such that
 *          floor(n*log(n))+1<=p<=floor((n+1)*log(n+1))-1.
 * https://oeis.org/A068362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer N such that there are exactly 4 primes p
 *            with floor(N·log(N))+1 ≤ p ≤ floor((N+1)·log(N+1))-1
 * Range:     u8 none, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/* --- Prime sieve ------------------------------------------------------ */
#define SIEVE_MAX 100000000ULL   /* enough for first 10 000 terms */

static uint8_t *sieve_bits = NULL;       /* bitset for odd numbers */
static size_t sieve_bytes = 0;           /* #bytes in sieve_bits */

static void init_sieve(void)
{
    if (sieve_bits != NULL) return;      /* already initialized */

    /* number of odd integers up to SIEVE_MAX   → bits needed  (SIEVE_MAX/2) */
    /* bytes needed  = ceil(bits/8)  */
    sieve_bytes = ((SIEVE_MAX >> 1) + 7) >> 3;
    sieve_bits = (uint8_t *)malloc(sieve_bytes);
    memset(sieve_bits, 0, sieve_bytes);

    /* simple sieve (only odd numbers stored) */
    size_t lim = (size_t)floor(sqrt((double)SIEVE_MAX));
    for (size_t i = 3; i <= lim; i += 2) {
        if (!(sieve_bits[(i >> 1) >> 3] & (1u << ((i >> 1) & 7)))) {
            for (size_t j = i * i; j <= SIEVE_MAX; j += i << 1) {
                sieve_bits[(j >> 1) >> 3] |= (1u << ((j >> 1) & 7));
            }
        }
    }
}

/* check whether n (n>=2) is prime using the sieve */
static inline bool is_prime_uint32(uint32_t n)
{
    if (n == 2) return true;
    if ((n & 1) == 0) return false;   /* even >2 */
    size_t idx = n >> 1;
    return !(sieve_bits[idx >> 3] & (1u << (idx & 7)));
}

/* --------------------------------------------------------------------- */

/* cache for sequence values */
static seq_t *seq_cache   = NULL;
static size_t seq_len     = 0;
static size_t seq_cap     = 0;

/* next integer to test */
static idx_t next_candidate = 1;

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    /* grow cache until it contains at least n terms */
    while (seq_len < (size_t)n) {
        /* compute the interval of primes for current candidate */
        double lowf  = floor((double)next_candidate * log((double)next_candidate)) + 1.0;
        double highf = floor((double)(next_candidate + 1) *
                            log((double)(next_candidate + 1))) - 1.0;
        uint32_t low  = (uint32_t)lowf;
        uint32_t high = (uint32_t)highf;

        /* count primes in [low,high] */
        uint32_t cnt = 0;
        for (uint32_t p = low; p <= high; ++p) {
            if (p > SIEVE_MAX) break;      /* out of sieve reach */
            if (is_prime_uint32(p)) ++cnt;
        }

        if (cnt == 4) {
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 4;
                seq_cache = (seq_t *)realloc(seq_cache,
                                             seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[(size_t)n - 1];
}
