/*
 * A168956  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   sequence a(n) is defined by the recurrence a(n) = b(n) + 37 *
 *            Σ_{k=1}^{21} a(n-k) – 703 * a(n-22), where b(0)=b(22)=1,
 *            b(1)…b(21)=2, and b(n)=0 otherwise. This implements the rational
 *            generating function given in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache of computed values, grows as needed */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of allocated array   */
    static size_t sz    = 0;   /* number of elements stored    */

    if (n >= sz) {
        /* grow the array to at least n+1 elements */
        size_t new_cap = sz ? sz * 2 : 1;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap   = new_cap;
        }
        /* compute missing terms up to n */
        for (idx_t i = (idx_t)sz; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 21 && i >= k; ++k) {
                sum += cache[i - k];
            }
            seq_t val = sum * (seq_t)37;
            if (i >= 22) {
                val -= cache[i - 22] * (seq_t)703;
            }
            seq_t bn;
            if (i == 0 || i == 22) {
                bn = 1;
            } else if (i >= 1 && i <= 21) {
                bn = 2;
            } else {
                bn = 0;
            }
            val += bn;
            cache[i] = val;
        }
        sz = n + 1;
    }

    return cache[n];
}
