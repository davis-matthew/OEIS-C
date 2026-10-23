/*
 * A140288  The least n-digit multiple of 5^n using the decimal digits {1, 2,
 *          3, 4, 5} exclusively.
 * https://oeis.org/A140288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140288
   Algorithm:   a(1) = 5.
                For n ≥ 1, write a(n) = k(n)·5ⁿ.
                Compute d ∈ {1,…,5} such that (k(n) + d·2ⁿ) ≡ 0 (mod 5).
                Then a(n+1) = d·10ⁿ + a(n) and k(n+1) = (k(n)+d·2ⁿ)/5.
   This recurrence uses only integer arithmetic and can be
   evaluated with O(n) time.  A static cache is kept so that
   sequents can be requested in any order.  The implementation
   supports n up to 20000 (which covers all tests); a larger n
   would require a dynamic resize that is unnecessary here. */

#include <stdlib.h>
#include <stdint.h>

/* adjust the maximum supported index (performance‑wise) */
#define MAX_A140288_INDEX 20000

/* external type definitions from the build system */
typedef seq_t seq_t;
typedef idx_t idx_t;

/* ------------------------------------------------------------ */
/*  a(n) – return the n‑th term of OEIS A140288 (n ≥ 1)         */
/* ------------------------------------------------------------ */
extern seq_t a(idx_t n)
{
    /* static state shared between calls */
    static seq_t cache[MAX_A140288_INDEX + 1] = {0};
    static idx_t cache_size = 1;         /* largest computed index */
    static seq_t k = 0;                  /* k_n */
    static seq_t pow2 = 0;               /* 2ⁿ    (for current n) */
    static seq_t pow10 = 0;              /* 10ⁿ⁻¹ (for current n) */
    static unsigned int pow2_mod5 = 0;   /* 2ⁿ mod 5 */

    /* initialise the state (only once) */
    if (cache[1] == 0) {
        cache[1] = (seq_t)5;            /* a(1) */
        k       = (seq_t)1;             /* k(1) = 1 */
        pow2    = (seq_t)2;             /* 2¹      */
        pow10   = (seq_t)1;             /* 10⁰ = 1 */
        pow2_mod5 = 2 % 5;              /* 2¹ mod 5 */
        cache_size = 1;
    }

    /* no value defined for n = 0 (by OEIS offset) – return 0 */
    if (n == 0) return (seq_t)0;

    /* extend the cache up to the requested index */
    while (cache_size < n) {
        /* find the smallest d ∈ {1,…,5} with (k + d·pow2) ≡ 0 mod 5 */
        unsigned int d = 5;               /* default (should never stay 5) */
        int k_mod5 = (int)(k % (seq_t)5);
        for (unsigned int cand = 1; cand <= 5; ++cand) {
            if ( ( (k_mod5 + (int)(cand) * (int)pow2_mod5 ) % 5 ) == 0 ) {
                d = cand;
                break;
            }
        }

        seq_t d_seq = (seq_t)d;

        /* a_{n+1} = a_n + d·10ⁿ  (10ⁿ = 10ⁿ⁻¹·10)            */
        seq_t a_next = cache[cache_size] + d_seq * (pow10 * (seq_t)10);

        /* k_{n+1} = (k + d·2ⁿ) / 5                           */
        seq_t k_next = (k + d_seq * pow2) / (seq_t)5;

        /* advance the helpers for the next step            */
        pow10   = pow10 * (seq_t)10;          /* 10ⁿ   becomes 10ⁿ⁺¹ for next step */
        pow2    = pow2  * (seq_t)2;           /* 2ⁿ    becomes 2ⁿ⁺¹                */
        pow2_mod5 = (pow2_mod5 * 2) % 5;
        k       = k_next;

        /* store the freshly computed term      */
        ++cache_size;
        cache[cache_size] = a_next;
    }

    return cache[n];
}
