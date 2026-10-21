/*
 * A063042  Collatz-2 (A063041) trajectory starting at 3.
 * https://oeis.org/A063042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {3,7,36,18,9}[n mod 5]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t cycle[5] = { (seq_t)3, (seq_t)7, (seq_t)36, (seq_t)18, (seq_t)9 };

seq_t a(idx_t n)
{
    return cycle[n % 5];
}
