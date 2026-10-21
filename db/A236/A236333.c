/*
 * A236333  The (n-2)-th (n>=3) triple of terms gives coefficients of double
 *          trinomial P_n(x) = ((n-2)^2*x^2 + n*x + 2)/2 (see comment).
 * https://oeis.org/A236333
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { n^2/9 if n ≡ 0 (mod 3); (n+5)/3 if n ≡ 1 (mod 3); 2 if
 *            n ≡ 2 (mod 3); } The offset is 3, i.e. a(3) = 1. The function
 *            can be used for any n >= 0; the values for n < 3 are irrelevant
 *            to the sequence specification
 * Range:     u8 n <= 47, u16 n <= 767, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 3 once. */
    idx_t r = n % 3u;
    if (r == 0u) {
        /* n is a multiple of 3, so n^2 is divisible by 9. */
        seq_t nn = (seq_t)n;
        return (nn * nn) / 9;
    } else if (r == 1u) {
        /* n ≡ 1 (mod 3), so n+5 is divisible by 3. */
        return ((seq_t)n + 5) / 3;
    } else {
        /* r == 2 (mod 3) */
        return (seq_t)2;
    }
}
