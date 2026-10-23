/*
 * A125388  Number of base 25 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the trace of the n‑th power of the 25×25 adjacency
 *            matrix where A[i][j] = 1 if |i-j| ≤ 5, otherwise 0. a(0) = 1.
 *            For n ≥ 1, a(n) counts closed walks of length n in this graph,
 *            i.e. number of base‑25 circular n‑digit numbers with adjacent
 *            digits differing by at most 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define BASE 25
#define MAXD 5        /* maximum distance */

static inline void
step(const seq_t in[BASE][BASE], seq_t out[BASE][BASE])
{
    for (int i = 0; i < BASE; i++) {
        for (int j = 0; j < BASE; j++) {
            seq_t sum = 0;
            int low  = j < MAXD ? 0 : j - MAXD;
            int high = j + MAXD >= BASE ? BASE - 1 : j + MAXD;
            for (int k = low; k <= high; k++)
                sum += in[i][k];
            out[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    /* two buffers for the dynamic programming */
    static seq_t cur[BASE][BASE];
    static seq_t nxt[BASE][BASE];

    /* initialise cur as the identity matrix (walks of length 0) */
    for (int i = 0; i < BASE; i++)
        for (int j = 0; j < BASE; j++)
            cur[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* iterate n times, each time extending walks by one step */
    for (idx_t stepcnt = 1; stepcnt <= n; stepcnt++) {
        step(cur, nxt);

        /* copy nxt to cur for next iteration */
        for (int i = 0; i < BASE; i++)
            for (int j = 0; j < BASE; j++)
                cur[i][j] = nxt[i][j];
    }

    /* sum the diagonal entries to obtain the trace */
    seq_t result = 0;
    for (int i = 0; i < BASE; i++)
        result += cur[i][i];

    return result;
}
