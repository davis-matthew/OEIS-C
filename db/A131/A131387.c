/*
 * A131387  Product of the nonzero digital products of n for all the bases 1
 *          to n (a 'total digital-product factorial').
 * https://oeis.org/A131387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{b=1}^{n} ∏_{d ∈ digits(n, b), d>0} d (product of all
 *            non‑zero digits of n in each base b from 1 to n)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;          /* base‑1 contribution is 1 */
    for (idx_t b = 2; b <= n; ++b)
    {
        idx_t temp = n;
        seq_t prod = 1;        /* product of non‑zero digits in base b */
        while (temp)
        {
            idx_t d = temp % b;
            if (d != 0)
                prod *= d;
            temp /= b;
        }
        result *= prod;
    }
    return result;
}
