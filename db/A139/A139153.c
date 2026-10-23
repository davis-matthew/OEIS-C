/*
 * A139153  a(n) = (n!+6)/6.
 * https://oeis.org/A139153
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! + 6)/6 (for n >= 3)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined – return 0 as a safe value. */
    if (n < 3) {
        return (seq_t)0;
    }

    /* n! / 6 = 1 for n = 3, and for n >= 4 it equals the product
       of the integers from 4 to n.  This avoids any intermediate
       overflow in seq_t that would occur if we multiplied all numbers
       up to n and divided afterwards. */
    seq_t prod = (seq_t)1;
    for (idx_t i = 4; i <= n; ++i) {
        prod *= (seq_t)i;   /* prod holds product of 4..i */
    }

    /* a(n) = n! / 6 + 1 */
    return prod + (seq_t)1;
}
