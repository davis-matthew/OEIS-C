/*
 * A196486  Irregular triangle 3^n-2^m.
 * https://oeis.org/A196486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 82, u32 n <= 323, u64 n <= 1280,
 *            u128 n <= 5096, big any n
 * Helpers:   bitlen_seq_t, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A196486: irregular triangle 3^n − 2^k.
   The sequence is the concatenation of rows:
   row r contains floor(log₂(3^r)) terms,
   the k‑th term of row r is 3^r − 2^k   (k = 1…floor(log₂(3^r))).
   This implementation walks rows until the input index is reached,
   then returns the corresponding term. */
#include "../utils/pow3.h"
#include "../utils/pow2.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    idx_t r = 1;
    seq_t p = pow3(r);
    unsigned rowlen = bitlen_seq_t(p) - 1;
    idx_t cum = rowlen;          /* total terms up to current row */

    while (cum < n) {
        ++r;
        p = pow3(r);
        rowlen = bitlen_seq_t(p) - 1;
        cum += (idx_t)rowlen;
    }

    idx_t prev_cum = cum - (idx_t)rowlen;
    idx_t k = n - prev_cum;          /* position within the row */
    seq_t pow2k = pow2(k);
    seq_t result = p - pow2k;
    return result;
}
