/*
 * A169052  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n = 0: a(0) = 1 for 1 ≤ n ≤ 23: a(n) = 39 · 38^(n‑1) for n ≥
 *            24: a(n) = (a(n‑1)+a(n‑2)+ … + a(n‑24))/19 This recurrence
 *            follows from the rational generating function of the Coxeter
 *            group described in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* 1 ≤ n ≤ 23 : words with no equal adjacents */
    if (n <= 23) {
        seq_t val = (seq_t)39;
        for (idx_t i = 1; i < n; ++i)
            val *= (seq_t)38;
        return val;
    }

    /* Initialise the first 24 terms (0 … 23) */
    seq_t buf[24];
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)39;
    for (idx_t i = 2; i < 24; ++i)
        buf[i] = buf[i - 1] * (seq_t)38;

    /* rolling sum of the last 24 values */
    seq_t sum = 0;
    for (idx_t i = 0; i < 24; ++i)
        sum += buf[i];

    idx_t pos = 0;                  /* position of the oldest element */
    for (idx_t k = 24; k <= n; ++k) {
        seq_t next = sum / (seq_t)19;  /* exact division */
        /* update the circular buffer and rolling sum */
        sum -= buf[pos];                /* remove oldest */
        buf[pos] = next;                /* insert newest */
        sum += next;                    /* add newest */
        pos = (pos + 1) % 24;           /* advance the window */
    }

    /* the last computed value is just before the current position */
    idx_t last = (pos + 23) % 24;
    return buf[last];
}
