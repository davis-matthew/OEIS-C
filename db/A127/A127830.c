/*
 * A127830  a(n) = Sum_{k=0..n} (binomial(floor(k/2),n-k) mod 2).
 * https://oeis.org/A127830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} ( C(floor(k/2), n-k) mod 2 )
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Rewriting the summation in terms of r = n – k.
       r ranges from 0 to ⌊n/3⌋; for each r check if
       binom(floor((n-r)/2), r) is odd, i.e. whether
       r & ~floor((n-r)/2) == 0. */
    for (idx_t r = 0; r <= n / 3; ++r)
    {
        idx_t m = (n - r) >> 1;          /* floor((n - r)/2) */
        if ((r & ~m) == 0)               /* binomial odd? */
            ++sum;
    }
    return sum;
}
