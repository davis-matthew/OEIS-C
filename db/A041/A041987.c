/*
 * A041987  Denominators of continued fraction convergents to sqrt(516).
 * https://oeis.org/A041987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A041987 – Denominators of continued fraction convergents to sqrt(516).
 * Formula:  a(0)=1, a(1)=1; for n>=2,
 * let p[n] be the n-th partial quotient of sqrt(516) (p[0]=22).
 * Then a(n)=p[n]*a(n-1)+a(n-2).
 * The function computes a(n) by iterating the continued fraction expansion
 * and memoizes all results for future calls.
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(516) */
    const uint64_t D   = 516ULL;
    const uint64_t a0  = 22ULL;   /* floor(sqrt(516)) */

    /* Static memoization tables */
    static seq_t  *qs = NULL;   /* denominators a(n)   */
    static uint64_t *mm = NULL; /* m_n values */
    static uint64_t *dd = NULL; /* d_n values */
    static uint64_t *aa = NULL; /* a_n values (partial quotients) */
    static idx_t max_n = 0;
    static int initialized = 0;

    if (!initialized) {
        /* Allocate for index 0 */
        qs = (seq_t*)malloc(sizeof(seq_t));
        mm = (uint64_t*)malloc(sizeof(uint64_t));
        dd = (uint64_t*)malloc(sizeof(uint64_t));
        aa = (uint64_t*)malloc(sizeof(uint64_t));
        /* Initial values for the continued fraction expansion */
        qs[0] = 1;
        mm[0] = 0;
        dd[0] = 1;
        aa[0] = a0;
        max_n = 0;
        initialized = 1;
    }

    if (n > max_n) {
        /* Enlarge tables to hold indices up to n */
        size_t new_size = (size_t)(n + 1);
        qs = (seq_t*)realloc(qs, new_size * sizeof(seq_t));
        mm = (uint64_t*)realloc(mm, new_size * sizeof(uint64_t));
        dd = (uint64_t*)realloc(dd, new_size * sizeof(uint64_t));
        aa = (uint64_t*)realloc(aa, new_size * sizeof(uint64_t));

        /* Compute new entries sequentially */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            uint64_t m_prev = mm[k - 1];
            uint64_t d_prev = dd[k - 1];
            uint64_t a_prev = aa[k - 1];

            uint64_t m = d_prev * a_prev - m_prev;
            uint64_t d = (D - m * m) / d_prev;
            uint64_t a = (a0 + m) / d;

            mm[k] = m;
            dd[k] = d;
            aa[k] = a;

            if (k == 1)
                qs[k] = (seq_t)a * qs[0];              /* q[-1] = 0 */
            else
                qs[k] = (seq_t)a * qs[k - 1] + qs[k - 2];
        }
        max_n = n;
    }

    return qs[n];
}
