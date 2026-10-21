/*
 * A235383  Fibonacci numbers that are the product of other Fibonacci numbers.
 * https://oeis.org/A235383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, fini, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235383: Fibonacci numbers that are the product of other Fibonacci numbers.
   Only two such numbers exist: F_6 = 8 and F_12 = 144.
   For n > 2, the sequence has no further terms, so a(n) = 0. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)144;
    return (seq_t)0;
}
