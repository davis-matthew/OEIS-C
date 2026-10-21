/*
 * A094284  a(n) = A094283(n+1)/A094283(n).
 * https://oeis.org/A094284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094284: a(n) = A094283(n+1)/A094283(n).
   The ratio stabilizes to 2 from n = 4 onward.
   Therefore a(1) = 5, a(2) = 3, a(3) = 3, and a(n) = 2 for all n >= 4. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2 || n == 3) return (seq_t)3;
    return (seq_t)2;
}
