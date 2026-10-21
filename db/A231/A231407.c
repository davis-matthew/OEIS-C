/*
 * A231407  Positive integers that are not the sum of distinct primes.
 * https://oeis.org/A231407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A231407: Positive integers that are not sums of distinct primes.
   Known terms: 1, 4, 6.  No further terms exist. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)4;
    if (n == (idx_t)3) return (seq_t)6;
    return (seq_t)0;
}
