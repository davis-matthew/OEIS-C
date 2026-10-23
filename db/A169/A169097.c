/*
 * A169097  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169097
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
   Formula (recurrence representation):
   a(0) = 1
   For 1 ≤ n ≤ 24:        a(n) = 2 + 34·(a(n-1)+…+a(n-24))
   For n = 25:            a(25) = 1 + 34·(a(24)+…+a(1)) – 595·a(0)
   For n ≥ 26:            a(n) = 34·(a(n-1)+…+a(n-24)) – 595·a(n-25)
*/
#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static idx_t last = (idx_t)-1;

    /* Allocate (or reallocate) storage */
    if (!seq) {
        size_t need = n + 1;
        if (need < 26) need = 26;          /* have enough room for first 25 terms */
        seq = (seq_t *)malloc(need * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)1;                 /* a(0) */
        cap = need;
        last = 0;
    }
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t pk;
        if (k == 0)          pk = (seq_t)1;
        else if (k <= 24)    pk = (seq_t)2;
        else if (k == 25)    pk = (seq_t)1;
        else                 pk = (seq_t)0;

#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value) {
            /* Integral seq_t: use 128‑bit intermediate to avoid overflow      */
            unsigned __int128 sum = 0;
            if (k <= 24) {
                for (idx_t i = 1; i <= k; ++i) {
                    sum += (unsigned __int128)seq[k-i] * (unsigned __int128)34;
                }
                seq[k] = (seq_t)((unsigned __int128)pk + sum);
            } else {
                for (idx_t i = 1; i <= 24; ++i) {
                    sum += (unsigned __int128)seq[k-i] * (unsigned __int128)34;
                }
                unsigned __int128 term = (unsigned __int128)seq[k-25] * (unsigned __int128)595;
                seq[k] = (seq_t)((unsigned __int128)pk + sum - term);
            }
        } else
#endif
        {
            /* Arbitrary‑precision seq_t: use its operator overloads            */
            seq_t sum = (seq_t)0;
            if (k <= 24) {
                for (idx_t i = 1; i <= k; ++i) {
                    sum += seq[k-i] * (seq_t)34;
                }
                seq[k] = pk + sum;
            } else {
                for (idx_t i = 1; i <= 24; ++i) {
                    sum += seq[k-i] * (seq_t)34;
                }
                seq[k] = pk + sum - seq[k-25] * (seq_t)595;
            }
        }
    }
    last = n;
    return seq[n];
}
