/*
 * A170572  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A170572 – Number of reduced words of length n in the Coxeter
 *  group with 35 generators where (S_i)^2 = (S_i S_j)^47 = I.
 *
 *  The generating function is
 *
 *          (t^47 + 2 t^46 + … + 2 t + 1)
 *  G(t) = ---------------------------------
 *           (561 t^47 – 33 t^46 – … – 33 t – 1)
 *
 *  Let Q(t) = 1 – 33 t – 33 t^2 – … – 33 t^46 + 561 t^47
 *          = Σ_{i=0}^{47} q_i t^i
 *      with  q_0 = 1,  q_i = –33 (1 ≤ i ≤ 46),  q_47 = 561.
 *
 *  Let P(t) = 1 + 2 t + … + 2 t^46 + t^47
 *          = Σ_{i=0}^{47} p_i t^i
 *      where  p_0 = 1,  p_i = 2 (1 ≤ i ≤ 46),  p_47 = 1.
 *
 *  From Q(t)·G(t) = P(t) we obtain the recurrence that holds for all
 *  n ≥ 0:
 *
 *          a(n) = p_n + 33 Σ_{i=1}^{46} a(n-i) – 561 a(n-47)
 *
 *  with the convention a(k) = 0 for k < 0.
 *  The first 47 values are generated automatically by the same
 *  recurrence (p_n is zero for n > 47).
 *
 *  The implementation stores the sequence in a static array that grows
 *  as needed and memoises all evaluated terms.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t used = 0;      /* number of elements stored   */
    static size_t cap   = 0;      /* allocated capacity          */

    /* grow cache if necessary */
    if (n >= used) {
        size_t newcap = (cap == 0) ? 64 : cap << 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – abort */
            abort();
        }
        cache = tmp;
        cap = newcap;
    }

    /* compute terms up to index n */
    while (used <= n) {
        idx_t k = used;            /* index of the term to compute */
        seq_t sum46 = 0;           /* Σ_{i=1}^{46} a(k-i)          */
        idx_t i;
        for (i = 1; i <= 46; ++i) {
            if (k >= i) {
                sum46 += cache[k - i];
            }
        }
        sum46 = sum46 * (seq_t)33;   /* 33 * sum                          */

        seq_t term47 = 0;           /* 561 * a(k-47)    */
        if (k >= 47) {
            term47 = (seq_t)561 * cache[k - 47];
        }

        seq_t pk = 0;               /* p_k from the numerator          */
        if (k == 0) {
            pk = (seq_t)1;
        } else if (k <= 46) {
            pk = (seq_t)2;
        } else if (k == 47) {
            pk = (seq_t)1;
        }

        seq_t val = pk + sum46 - term47;
        cache[used++] = val;
    }

    return cache[n];
}
