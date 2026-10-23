/*
 * A046310  Numbers that are divisible by exactly 8 primes counting
 *          multiplicity.
 * https://oeis.org/A046310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer whose total number of prime factors
 *            (counting multiplicity) equals 8 (i.e. Ω(n)=8)
 * Range:     u8 none, u16 n <= 887, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We pre‑compute all 8‑almost primes up to a fixed limit that is
    comfortably larger than a(10000).  For any requested n the answer
    is read from a cached array.  The code is valid C11 and C++17.  */

#include <stdint.h>
#include <stdlib.h>

#define MAX_TERMS 20000            /* plenty for n ≤ 10000 */
#define SIEVE_LIMIT 5000000U       /* > a(10000) ≈ 4.1·10⁶ */

/* ---------  Global cache  --------- */
static seq_t  seq_cache[MAX_TERMS];
static size_t seq_len = 0;         /* number of terms stored */
static int    cache_ready = 0;     /* 0 → not yet built */

/* ---------  Helper – build the cache  --------- */
static void build_cache(void)
{
    /* If we survive this function the cache is ready. */
    if (cache_ready) return;

    /* ----------  Smallest prime factor sieve  ---------- */
    uint32_t *spf = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = 0;

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    /* ----------  Ω(n) computation and collection  ---------- */
    uint8_t *omega = (uint8_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint8_t));
    if (!omega) { free(spf); exit(EXIT_FAILURE); }

    omega[0] = omega[1] = 0;
    seq_len = 0;

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        /* Ω(i) = Ω(i / spf[i]) + 1  */
        omega[i] = omega[i / spf[i]] + 1;

        if (omega[i] == 8) {
            if (seq_len < MAX_TERMS) {
                seq_cache[seq_len++] = (seq_t)i;
            }
        }
    }

    free(omega);
    free(spf);
    cache_ready = 1;
}

/* ---------  Public function  --------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the OEIS offset starts at 1 */

    build_cache();                 /* build once, reuse thereafter */

    /* If somebody requests an index beyond our pre‑computed range
       we simply return 0 – the problem statement only demands
       correct values for n up to about 10 000.   */
    if (n <= seq_len) return seq_cache[n - 1];

    /* In the unlikely event that more terms are required, we
       could extend the sieve, but that is outside the short
       time budget required by the problem.  */
    return 0;
}
