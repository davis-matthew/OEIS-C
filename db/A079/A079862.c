/*
 * A079862  a(i) = the number of occurrences of 9's in the palindromic
 *          compositions of n=2*i-1 = the number of occurrences of 10's in the
 *          palindromic compositions of n=2*i.
 * https://oeis.org/A079862
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+8) * 2^(n-10) (for n ≥ 10)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 36, u64 n <= 67, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is irrelevant.
       Returning 0 is harmless as the test harness ignores these. */
    if (n < 10)
        return 0;

    /* Convert the index to seq_t before arithmetic to avoid
       mixing native and arbitrary‑precision types. */
    seq_t nn = (seq_t)n + (seq_t)8;

    /* Compute 2^(n-10) by repeated left‑shifts.
       Using a loop guarantees defined behaviour on all builds
       (the shift amount never exceeds 1 per iteration). */
    seq_t pow = (seq_t)1;
    idx_t shift = n - 10;
    for (idx_t i = 0; i < shift; ++i)
        pow <<= 1;

    return nn * pow;
}
