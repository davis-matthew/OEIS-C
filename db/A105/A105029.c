/*
 * A105029  Write numbers in binary under each other, left justified, read
 *          diagonals in downward direction, convert to decimal.
 * https://oeis.org/A105029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer obtained by reading the n‑th down‑right *
 *            diagonal of the table whose rows are the binary representations
 *            * of 0,1,2,… written left‑justified, then interpreting the
 *            collected * bits as a binary number. Example: a(0)=0, a(1)=2,
 *            a(2)=6, a(3)=5,
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9761 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* number of bits needed to represent x; 0 counts as one bit. */
static inline idx_t bitlen_idx(idx_t x)
{
    if (x == 0) return 1;
    idx_t l = 0;
    while (x) {
        ++l;
        x >>= 1;
    }
    return l;
}

/* value on the n-th diagonal */
static seq_t diag_value(idx_t n)
{
    seq_t val = 0;
    for (idx_t r = n; ; ++r) {
        idx_t len = bitlen_idx(r);
        idx_t pos = r - n;
        if (pos >= len) break;
        unsigned bit = (unsigned)((r >> (len - 1 - pos)) & 1U);
        val = (val << 1) | (seq_t)bit;
    }
    return val;
}

seq_t a(idx_t n)
{
    return diag_value(n);
}
