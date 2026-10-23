/*
 * A168854  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168854
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

/* Sequence A168854 (OEIS)
   Recurrence:
   For n = 0:  a(0) = 1
   For 1 ≤ n ≤ 19:  a(n) = 2 + 31 * Σ_{k=1}^{n} a(n−k)
   For n ≥ 20:      a(n) = 31 * Σ_{k=1}^{19} a(n−k) – 496 * a(n−20)
   plus a(20) receives +1 (the numerator coefficient for t^20).
   This is equivalent to the generating‑function representation in the OEIS entry. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *buf = NULL;
    static idx_t  len = 0;   /* number of cached elements (buf[0..len-1]) */

    if (n >= len) {
        /* Grow the buffer to size n+1 */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_len * sizeof(seq_t));
        if (!tmp) abort(); /* memory allocation failed */
        buf = tmp;

        /* Compute sequence entries from old len up to n */
        for (idx_t i = len; i <= n; ++i) {
            if (i == 0) {
                buf[0] = (seq_t)1;          /* a(0) = 1 */
            } else if (i < 20) {
                /* a(i) = 2 + 31 * Σ_{k=1}^{i} a(i−k) */
                seq_t sum = (seq_t)0;
                for (idx_t k = 1; k <= i; ++k) {
                    sum += buf[i - k];
                }
                seq_t val = sum;
                val *= (seq_t)31;
                val += (seq_t)2;
                buf[i] = val;
            } else {
                /* a(i) = 31 * Σ_{k=1}^{19} a(i−k) – 496 * a(i−20) + p
                   with p = 1 if i==20 else 0 */
                seq_t sum = (seq_t)0;
                for (idx_t k = 1; k <= 19; ++k) {
                    sum += buf[i - k];
                }
                seq_t val = sum;
                val *= (seq_t)31;

                seq_t sub = buf[i - 20];
                sub *= (seq_t)496;
                val -= sub;

                if (i == 20) {
                    val += (seq_t)1;
                }
                buf[i] = val;
            }
        }
        len = n + 1;
    }

    return buf[n];
}
