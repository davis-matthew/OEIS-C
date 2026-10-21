/*
 * A052634  Expansion of e.g.f. 1/((1-2*x^2)*(1-x)).
 * https://oeis.org/A052634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * (2^(floor(n/2)+1) - 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for the OEIS sequence A052634.
 * Uses a static factorial cache to avoid recomputing factorials.
 * The power of two is computed by repeated doubling. */
seq_t a(idx_t n)
{
    /* static cache for factorials : fact[i] = i!   */
    static seq_t *fact = NULL;
    static idx_t fact_size = 0;   /* number of entries stored ( = last computed index + 1 ) */

    /* extend factorial cache if necessary */
    if (n >= fact_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(fact, new_size * sizeof(seq_t));
        if (!tmp) return 0;      /* out‑of‑memory: return 0 (undefined, but
                                    not expected in tests) */
        fact = tmp;

        /* initialize from existing size */
        if (fact_size == 0) {       /* first call */
            fact[0] = 1;            /* 0! = 1 */
            fact_size = 1;
        }
        for (idx_t i = fact_size; i <= n; ++i)
            fact[i] = fact[i-1] * (seq_t)i;

        fact_size = new_size;
    }

    /* compute 2^( floor(n/2)+1 ) */
    idx_t k = n/2 + 1;   /* floor(n/2)+1 */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow2 *= 2;

    seq_t term = pow2 - 1;
    return fact[n] * term;
}
