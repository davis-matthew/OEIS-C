/*
 * A168776  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1; * For n ≥ 0: * a(n) = (n <= 18 ? 1 : 0) + 2 * Σ_{k
 *            odd, 1≤k≤18} a(n-k) - Σ_{k even, 2≤k≤18} a(n-k) * (Indices < 0
 *            are interpreted as 0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /*  Store only the last 18 values; buf[i] holds the value a(i) for the
     *  most recent computation of i (indices are taken modulo 18). */
    static seq_t buf[18] = {0};

    for (idx_t i = 0; i <= n; ++i) {
        seq_t val = (i <= 18) ? (seq_t)1 : (seq_t)0;   /* RHS for this i */

        for (idx_t k = 1; k <= 18; ++k) {
            if (i >= k) {
                seq_t prev = buf[(i - k) % 18];
                if (k & 1)          /* odd k  →  +2*prev */
                    val += (seq_t)2 * prev;
                else                /* even k →  -prev  */
                    val -= prev;
            }
        }
        buf[i % 18] = val;   /* store a(i) */
    }

    return buf[n % 18];
}
