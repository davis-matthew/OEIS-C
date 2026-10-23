/*
 * A123390  Triangle read by rows: n-th row starts with n and continues with
 *          half the previous value as long as that is even.
 * https://oeis.org/A123390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = value in the row that starts with r = n-th row starting
 *            number. Row r starts with r and continues halving while the
 *            value is even. The row length is v2(r)+1 (where v2 is the
 *            exponent of 2 in r). Thus for the n-th term (1‑based) we find
 *            the row r such that the cumulative number of terms up...
 * Range:     u8 n <= 502, u16 n <= 9973, u32 n <= 9973, u64 n <= 9973,
 *            u128 n <= 9973, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t trailing_zeros(idx_t x) {
    idx_t c = 0;
    while ((x & 1) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    /* n is 1‑based; the sequence is defined for n >= 1 */
    idx_t cumulative = 0;
    for (idx_t r = 1; ; ++r) {
        idx_t len = trailing_zeros(r) + 1;
        if (cumulative + len >= n) {
            idx_t pos = n - cumulative - 1;  /* 0‑based position inside the row */
            return ((seq_t)r) >> pos;
        }
        cumulative += len;
    }
}
