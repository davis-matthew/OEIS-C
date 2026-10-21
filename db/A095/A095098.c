/*
 * A095098  Fib001 numbers: those k for which the Zeckendorf expansion
 *          A014417(k) ends with two zeros and a final one.
 * https://oeis.org/A095098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*floor((n+1)*phi)+n-1, phi = (1+sqrt(5))/2
 * Range:     u8 none, u16 none, u32 n <= 3549, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = n+1, computed in seq_t */
    seq_t k = (seq_t)n + 1;

    /* Two consecutive convergents of phi:
       832040/514229 < phi < 1346269/832040   (indices 28 and 29) */
    static const seq_t num1 = 832040;
    static const seq_t den1 = 514229;
    static const seq_t num2 = 1346269;
    static const seq_t den2 = 832040;

    /* Compute floor(k * phi) using lower and upper rational bounds.
       The two values differ by at most 1; if they differ we take the larger
       one, which must be the true floor. */
    seq_t low  = (k * num1) / den1;
    seq_t high = (k * num2) / den2;
    seq_t floor_phi = (low == high ? low : high);

    /* a(n) = 2*floor_phi + n - 1 */
    return floor_phi * 2 + (seq_t)n - 1;
}
