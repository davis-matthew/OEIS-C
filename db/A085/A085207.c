/*
 * A085207  Array A(x,y): concatenation of binary expansions of x & y, listed
 *          antidiagonalwise as A(0,0), A(1,0), A(0,1), A(2,0), A(1,1),
 *          A(0,2), ... Zero is expanded as an empty string.
 * https://oeis.org/A085207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = concatenation of binary expansions of the pair (x,y)
 *            occurring at index n in the antidiagonal enumeration of the
 *            array A(x,y) where A(x,y) = concat(bin(x), bin(y)), zero is
 *            represented by an empty string
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt128.h"

static inline size_t bitlen_seq(seq_t v) {
    size_t len = 0;
    while (v) {
        v >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n) {
    /* compute s = floor((sqrt(8*n+1)-1)/2) */
    unsigned __int128 sq = floor_sqrt128((unsigned __int128)n * 8 + 1);
    unsigned __int128 s128 = (sq - 1) / 2;
    idx_t s = (idx_t)s128;                    /* diagonal index */

    /* starting index of this diagonal: s*(s+1)/2 */
    unsigned __int128 diag_start128 = (unsigned __int128)s * (s + 1) / 2;
    idx_t diag_start = (idx_t)diag_start128;

    idx_t offset = n - diag_start;           /* position within diagonal */
    idx_t x = s - offset;                    /* row */
    idx_t y = offset;                        /* column */

    size_t leny = bitlen_seq((seq_t)y);      /* bit length of y */
    return (((seq_t)x) << leny) | ((seq_t)y);
}
