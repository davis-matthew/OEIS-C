/*
 * A131051  a(n) = 5*2^(n-2) - 2 for n > 1, with a(1) = 1.
 * https://oeis.org/A131051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^(n-2) - 2 for n > 1, a(1) = 1 (offset 1, so a(1) is
 *            the first value)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    /* compute 2^(n-2) using a left shift (works for all unsigned types,
       including the arbitrary‑precision type used in the C++ build) */
    seq_t pow2 = (seq_t)1 << (n - 2);

    /* 5 * 2^(n-2) - 2 */
    seq_t val = pow2 * (seq_t)5;
    val -= (seq_t)2;
    return val;
}
