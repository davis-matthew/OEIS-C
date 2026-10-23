/*
 * A105202  Irregular triangle read by rows: row n gives the word
 *          f(f(f(...(1)))) [with n applications of f], where f is the
 *          morphism 1->{1,2,1}, 2->{2,3,2}, 3->{3,1,3}.
 * https://oeis.org/A105202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th term of the irregular triangle * read by rows.
 *            Each row r is obtained by applying the * morphism * 1 → {1,2,1},
 *            2 → {2,3,2}, 3 → {3,1,3} * r times to the single symbol 1. The
 *            sequence is the * concatenation of all rows (offset 0). This
 *            implementation * computes a(n) by locating t...
 * Range:     u8 n <= 9840, u16 n <= 9840, u32 n <= 9840, u64 n <= 9840,
 *            u128 n <= 9840, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that the first element of row r is at
     * index 'start' and the row has length 3^r.  We maintain
     * start and len in 128‑bit integers to avoid overflow. */
    idx_t r      = 0;
    __int128 start = 0;   /* starting index of the current row */
    __int128 len   = 1;   /* length of the current row = 3^r */

    while (start + len <= (__int128)n) {
        start += len;               /* move to next row's start index */
        len   *= 3;                 /* next row length = 3 * previous */
        ++r;
    }

    /* Column index within the row (0‑based) */
    idx_t c = n - (idx_t)start;

    /* Decode the column index in base 3, following the morphism.
     * We keep the current symbol as a small integer 1,2,3
     * and perform the mapping iteratively. */
    unsigned char symbol = 1;        /* value before any mapping */
    if (r > 0) {
        __int128 power = 1;
        for (int i = 0; i < r - 1; ++i)  /* power = 3^(r-1) */
            power *= 3;

        for (int i = 0; i < r; ++i) {
            unsigned d = (unsigned)(((__int128)c / power) % 3);
            /* apply the morphism for the current symbol */
            if (symbol == 1) {
                symbol = (d == 1) ? 2 : 1;
            } else if (symbol == 2) {
                symbol = (d == 1) ? 3 : 2;
            } else { /* symbol == 3 */
                symbol = (d == 1) ? 1 : 3;
            }
            power /= 3;          /* move to next less significant digit */
        }
    }

    return (seq_t)symbol;
}
