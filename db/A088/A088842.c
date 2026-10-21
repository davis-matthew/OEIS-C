/*
 * A088842  Denominator of the quotient sigma(7n)/sigma(n).
 * https://oeis.org/A088842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, frac
 * Formula:   a(n) = (7^(v7(n)+1)-1)/6, where v7(n) is the exponent of 7 in n.
 *            This follows from the multiplicative formula a(7^e) =
 *            (7^(e+1)-1)/6 and a(p^e)=1 for p≠7
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; undefined for 0 but return 0 to stay safe */
        return 0;

    /* Count exponent of 7 in n */
    idx_t temp = n;
    idx_t v = 0;
    while (temp % 7 == 0) {
        ++v;
        temp /= 7;
    }

    /* Compute 7^(v+1) in seq_t to avoid overflow. */
    seq_t pow = 1;
    for (idx_t i = 0; i < v + 1; ++i)
        pow *= (seq_t)7;

    /* (pow - 1) is always divisible by 6. */
    seq_t res = (pow - (seq_t)1) / (seq_t)6;
    return res;
}
