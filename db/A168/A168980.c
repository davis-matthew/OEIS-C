/*
 * A168980  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168980: Number of reduced words of length n in
    a Coxeter group on 15 generators with exchange relations
    S_i^2 = (S_i S_j)^23 = I (i != j).

    The generating function is
          P(t) = t^23 + 2*t^22 + ... + 2*t + 1
          Q(t) = 1 - 13*t - 13*t^2 - ... - 13*t^22 + 91*t^23

    Hence a(n) satisfies the linear recurrence
          a(n) = 13*a(n‑1) + ... + 13*a(n‑22) – 91*a(n‑23) + p_n
    where p_n = 1 if n = 0 or n = 23, p_n = 2 if 1 ≤ n ≤ 22,
    and p_n = 0 for n > 23.  */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#  include <new>          /* for placement new */
#endif

/* Helper macro to construct a seq_t object in C++ (no‑op in C). */
#ifdef __cplusplus
#  define SEQ_T_CONSTRUCT(p)   new (p) seq_t()
#else
#  define SEQ_T_CONSTRUCT(p)   do {} while (0)
#endif

seq_t a(idx_t n)
{
    /* same static cache for all calls */
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;     /* allocated capacity */
    static size_t  len     = 0;     /* number of terms stored (indices 0..len‑1) */

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failure: abort */
        /* construct newly allocated elements (C++ only) */
        for (size_t i = cap; i < newcap; ++i) SEQ_T_CONSTRUCT(tmp + i);
        cache = tmp;
        cap = newcap;
    }

    /* compute terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = 0;

        /* terms a(i‑1) … a(i‑22) multiplied by 13 */
        for (int k = 1; k <= 22; ++k) {
            if (i >= k) {
                sum += cache[(size_t)(i - k)] * (seq_t)13;
            }
        }

        /* subtract 91*a(i‑23) if present */
        if (i >= 23) {
            sum -= cache[(size_t)(i - 23)] * (seq_t)91;
        }

        /* add the inhomogeneous term p_i */
        seq_t p = 0;
        if (i == 0 || i == 23) {
            p = 1;
        } else if (i <= 22) {
            p = 2;
        }
        cache[(size_t)i] = sum + p;
        ++len;
    }

    return cache[(size_t)n];
}
