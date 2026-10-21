/*
 * A080109  Square of primes of the form 4k+1 (A002144).
 * https://oeis.org/A080109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n)^2 where p_n is the n-th prime of the form 4k+1
 *            (OEIS A080109). The algorithm builds primes incrementally by
 *            trial division using all primes found so far. It keeps two
 *            static arrays: 1) `primes` – all primes discovered (needed for
 *            primality tests) 2) `primes1` – primes ≡ 1 (mod 4); th...
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 3257, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Public function signature – provided by the build system. */
seq_t a(idx_t n);

/* ------------------------------------------------------------------------ */
/*   Static data – stored once for all calls to a()                          */
/* ------------------------------------------------------------------------ */

/* Maximum number of primes that could be needed for n up to about 100 000
   This is comfortably larger than required for the test ranges (≤ 10000). */
#define MAX_ALL_PRIMES   200000
#define MAX_1MOD4_PRIMES 10000

static uint32_t primes[MAX_ALL_PRIMES];          /* all primes discovered  */
static size_t   primes_len   = 0;                /* current length of primes array */

static uint32_t primes1[MAX_1MOD4_PRIMES];       /* primes ≡ 1 (mod 4)      */
static size_t   primes1_len  = 0;                /* current length of primes1 array */

static uint32_t last_candidate = 1;              /* last integer checked for primality */

/* ------------------------------------------------------------------------ */
/*   Helper: test whether a candidate is prime using the primes array      */
/* ------------------------------------------------------------------------ */
static int is_prime_candidate(uint32_t cand)
{
    /* 2 and 3 are handled automatically by the loop below. */
    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > cand) break;
        if (cand % p == 0) return 0;
    }
    return 1;
}

/* ------------------------------------------------------------------------ */
/*   Helper: generate primes until we have at least `target` 1‑mod‑4 primes */
/* ------------------------------------------------------------------------ */
static void generate_primes_1mod4_up_to(size_t target)
{
    for (;;) {
        ++last_candidate;
        if (last_candidate < 2) continue;
        if ((last_candidate & 1u) == 0) {          /* skip even numbers >2 */
            if (last_candidate == 2 && primes_len == 0) {
                /* 2 is the first prime */
                primes[primes_len++] = 2;
            }
            continue;
        }
        if (is_prime_candidate(last_candidate)) {
            primes[primes_len++] = last_candidate;
            if ((last_candidate & 3u) == 1) {     /* cand ≡ 1 (mod 4) */
                if (primes1_len < MAX_1MOD4_PRIMES) {
                    primes1[primes1_len++] = last_candidate;
                }
            }
        }
        if (primes1_len >= target) break;
    }
}

/* ------------------------------------------------------------------------ */
/*   Main sequence function                                                   */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n = 0, but avoid undefined behaviour */

    /* Ensure we have enough 1‑mod‑4 primes generated. */
    if (primes1_len < (size_t)n) {
        generate_primes_1mod4_up_to((size_t)n);
    }

    uint32_t p = primes1[(size_t)n - 1];
    return (seq_t)p * (seq_t)p;
}
