/*
 * A169447  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: Let S(n) = Σ_{k=1}^{31} a(n-k) (treat a(m)=0
 *            if m<0) If 1 ≤ n ≤ 31: a(n) = 48·S(n) + 2 If n = 32: a(32) =
 *            48·S(32) – 1176·a(0) + 1 If n ≥ 33: a(n) = 48·S(n) –
 *            1176·a(n-32) This recurrence follows from the generating
 *            function G(t) = (1+2∑_{k=1}^{31}t^k+t^32)/(1–48∑_{k=1}^{31}t^...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic array to cache computed terms. */
    static seq_t *seq_arr = NULL;
    static idx_t  seq_len = 0;      /* number of terms stored (index+1) */
    static size_t seq_cap = 0;      /* current allocated capacity */

    /* Ensure we have memory for at least n+1 elements. */
    if (seq_len == 0) {
        /* First call: initialise with a(0). */
        seq_cap = 1;
        seq_arr = (seq_t*)realloc(NULL, seq_cap * sizeof(seq_t));
        if (!seq_arr) abort();          /* allocation failed */
        seq_arr[0] = (seq_t)1;          /* a(0) = 1 */
        seq_len = 1;
    }
    if (n + 1 > seq_len) {
        /* Grow the array up to index n. */
        size_t needed = (size_t)(n + 1);
        if (needed > seq_cap) {
            size_t newcap = seq_cap ? seq_cap << 1 : 1;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t*)realloc(seq_arr, newcap * sizeof(seq_t));
            if (!tmp) abort();
            seq_arr = tmp;
            seq_cap = newcap;
        }
        for (idx_t i = seq_len; i <= n; ++i) {
            seq_t sum = 0;
            /* Sum of the previous up to 31 terms. */
            for (idx_t k = 1; k <= 31 && i >= k; ++k)
                sum += seq_arr[i - k];
            seq_t val = sum * (seq_t)48;
            if (i >= 32)
                val -= (seq_t)1176 * seq_arr[i - 32];
            if (i >= 1 && i <= 31)
                val += (seq_t)2;
            else if (i == 32)
                val += (seq_t)1;
            seq_arr[i] = val;
        }
        seq_len = n + 1;
    }
    return seq_arr[n];
}
