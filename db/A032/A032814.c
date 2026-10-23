/*
 * A032814  Numbers whose set of base-14 digits is {2,3}.
 * https://oeis.org/A032814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer whose base‑14 representation
 *            uses only the digits 2 and 3. The first digit is always 2 or 3,
 *            and the sequence is sorted by increasing value. This
 *            implementation finds the length L of the base‑14 number, then
 *            interprets the n‑th number as a binary word of length L...
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 766, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 (behaviour for n < 1 is undefined) */
    idx_t len = 1;                  /* current number of digits */
    idx_t cum = 2;                  /* numbers with up to len digits:
                                      2^(len+1) - 2 ; start with len=1 */

    /* Find the length L such that 2^(L+1)-2 >= n.  cum holds the
       cumulative count after adding 2^len for the current len. */
    while (cum < n) {
        ++len;
        cum += (idx_t)1 << len;     /* add 2^len numbers of this length */
    }

    /* Numbers with exactly len digits start after prev_cum numbers */
    idx_t prev_cum = cum - ((idx_t)1 << len);
    idx_t offset   = n - prev_cum - 1;   /* 0‑based index among len‑digit numbers */

    /* Construct the value by interpreting the bits of offset:
       most significant bit → most significant digit. */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i) {
        val = val * (seq_t)14;
        if ((offset >> (len - 1 - i)) & 1) {
            val += (seq_t)3;           /* bit 1 → digit 3 */
        } else {
            val += (seq_t)2;           /* bit 0 → digit 2 */
        }
    }
    return val;
}
