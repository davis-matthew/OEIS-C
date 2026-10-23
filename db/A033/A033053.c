/*
 * A033053  Numbers whose base-2 representation Sum_{i=0..m} d(i)*2^i has
 *          d(i)=1 when i != m mod 2.
 * https://oeis.org/A033053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the numbers whose binary representation has all bits at
 *            positions i that are not congruent to m (the index of the most
 *            significant bit) modulo 2 set to 1, where m = floor(log2(n))
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 9765, u64 n <= 9765,
 *            u128 n <= 9765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined behaviour for n < 1 */

    /* Find the block (value of m) that contains the nth term */
    idx_t cumulative = 0;
    idx_t m = 0;
    while (1) {
        idx_t cnt;
        if ((m & 1) == 0) {              /* even m */
            cnt = (idx_t)1 << (m / 2);
        } else {                         /* odd m */
            cnt = (idx_t)1 << ((m - 1) / 2);
        }
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++m;
    }

    /* Index within the current block (0‑based) */
    idx_t offset = n - cumulative - 1;

    /* Size of the underlying integer type in bits */
    size_t bitlen = sizeof(seq_t) * (size_t)CHAR_BIT;

    /* Build the value */
    seq_t value = 0;

    /* Set the forced bits: the most significant bit (pos == m)
       and all lower bits with parity different from m. */
    for (idx_t pos = 0; pos <= m && pos < (idx_t)bitlen; ++pos) {
        if (pos == m || ((pos & 1) != (m & 1))) {
            value |= ((seq_t)1 << pos);
        }
    }

    /* Insert the free bits (same parity as m) from the offset */
    idx_t rem = offset;
    for (idx_t pos = 0; pos < m && pos < (idx_t)bitlen; ++pos) {
        if ((pos & 1) == (m & 1)) {      /* free position */
            if (rem & 1) {
                value |= ((seq_t)1 << pos);
            }
            rem >>= 1;
        }
    }

    return value;
}
