/*
 * A091882  Expansion of g.f. (1-10x)/(1-15x).
 * https://oeis.org/A091882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15^n / 3 (n > 0), a(0) = 1. The expression 15^n/3 is
 *            integer for all n>0 because 15^n = 3^n·5^n is divisible by 3.
 *            The implementation multiplies by 15 exactly n times, then
 *            divides by 3. For n==0 it returns 1 directly. No auxiliary data
 *            structures are required and the routine is O(n). It w...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset value a(0).  For n==0 the closed form gives
       15^0/3 + 2*0^0/3 = 1.  Returning immediately avoids a division
       by 3 when the result should be an integer. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute 15^n by repeated multiplication.  The intermediate
       results never exceed the final value, so no overflow can occur
       in a build where the final value fits in seq_t.  For builds with
       arbitrary‑precision seq_t this works for any n. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        result *= (seq_t)15;
    }

    /* Since 15^n is divisible by 3 for n>0, the division is exact. */
    result /= (seq_t)3;
    return result;
}
