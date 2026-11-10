/*
 * A208569  Duplicate of A131271.
 * https://oeis.org/A208569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A131271(n-1); T(0,1)=1, T(j,2k-1)=T(j-1,k),
 *            T(j,2k)=2^j+1-T(j-1,k), read by rows (row j has 2^j terms)
 * Range:     i8 n <= 128, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Validated: standard, pass, 89 terms through n = 7734 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t m = n - 1;                 /* 0-based index into A131271 */
    unsigned r = 0;
    while (((idx_t)1 << (r + 1)) - 1 <= m) r++;   /* 2^r - 1 <= m < 2^(r+1) - 1 */
    idx_t k0 = m - (((idx_t)1 << r) - 1);         /* 0-based column in row r */
    seq_t v = 1;
    seq_t p = 1;
    for (unsigned j = 1; j <= r; j++) {
        p = p * 2;                                /* 2^j */
        idx_t kj = k0 >> (r - j);                 /* 0-based column at row j */
        if (kj & 1) v = p + 1 - v;                /* even 1-based position */
    }
    return v;
}
