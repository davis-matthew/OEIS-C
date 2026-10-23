/*
 * A182439  Table a(k,i), read by antidiagonals, in which the n-th row
 *          comprises A214206(n) in position 0 followed by a second order
 *          recursive series G in which each product G(i)*G(i+1) lies in the
 *          same row of A001477 (interpreted as a square array - see below).
 * https://oeis.org/A182439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A182439
    Tabulated as a(k,i) read by antidiagonals.
    a(k,0) = largest m with m(m+1)/2 ≤ 14·k     (A003056(14k))
    a(k,1) = k
    a(k,2) = 14
    For i ≥ 3: a(k,i) = 6·a(k,i–1) – a(k,i–2) + G_k
                where  G_k = 26 + 2·k – 4·a(k,0)
    (The same recurrence a(k,i)=7a(k,i–1)–7a(k,i–2)+a(k,i–3) holds.)
    The sequence a(n) is read off the table by antidiagonals:
    d = 0,1,2,… ;  for a fixed d the terms are
        (row=d, col=0), (row=d–1, col=1), … , (row=0, col=d)
*/

#include <stdint.h>
#include <math.h>
#include "../utils/floor_sqrt.h"

static seq_t a_row_col(idx_t row, idx_t col)
{
    /* compute a(row, col) */
    /* first compute a(row,0) */
    seq_t a0;
    {
        seq_t val = (seq_t)14 * (seq_t)row;          /* 14·row   */
        seq_t x   = (seq_t)8 * val + (seq_t)1;       /* 8·val+1  */
        seq_t s   = floor_sqrt(x);                   /* sqrt(8·val+1) */
        a0 = (s - (seq_t)1) / (seq_t)2;             /* floor((sqrt-1)/2) */
    }

    if (col == 0) return a0;
    if (col == 1) return (seq_t)row;
    if (col == 2) return (seq_t)14;

    /* G(row) = 26 + 2·row – 4·a0 */
    seq_t G = (seq_t)26 + (seq_t)2 * (seq_t)row - (seq_t)4 * a0;

    /* iterate recurrence */
    seq_t a_prev2 = a0;          /* a(i,0)   */
    seq_t a_prev1 = (seq_t)row;  /* a(i,1)   */
    seq_t a_cur   = (seq_t)14;   /* a(i,2)   */

    for (idx_t t = 3; t <= col; ++t) {
        seq_t next = (seq_t)6 * a_cur - a_prev1 + G;
        a_prev1 = a_cur;
        a_cur   = next;
    }
    return a_cur;
}

seq_t a(idx_t n)
{
    /* find the diagonal d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t low  = 0;
    idx_t high = (idx_t)(sqrt(2.0 * (double)n) + 3.0) + 2;  /* safe upper bound */

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t d = low;
    idx_t before = d * (d + 1) / 2;      /* terms before diagonal d */
    idx_t offset = n - before;           /* 0‑based position on diagonal d */
    idx_t row = offset;
    idx_t col = d - offset;
    return a_row_col(row, col);
}
