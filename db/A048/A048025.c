/*
 * A048025  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{⌊n/6⌋} binomial(⌊(n-2)/3⌋, t) *
 *            binomial(n-⌊(n-2)/3⌋, 5t)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         where m = ⌊(n-2)/3⌋.  */

/* Helper to compute GCD of two seq_t values. */
static seq_t _gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Multiply‑then‑divide binomial coefficient using exact division to avoid overflow. */
static seq_t _comb(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t a = n - k + i;   /* numerator part */
        seq_t b = i;           /* denominator part */

        seq_t g = _gcd_seq(a, b);
        a /= g;
        b /= g;

        /* res is guaranteed to be divisible by b at each step */
        res /= b;
        res *= a;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* lower part size */
    idx_t m = (n >= 2) ? (n - 2) / 3 : 0;

    /* upper part size */
    idx_t upper = n - m;

    /* t must satisfy: 1 ≤ t ≤ ⌊n/6⌋, t ≤ m, 5t ≤ upper */
    idx_t max_t = n / 6;
    if (m < max_t)
        max_t = m;
    if ((upper / 5) < max_t)
        max_t = upper / 5;

    if (max_t == 0)
        return (seq_t)0;

    seq_t total = 0;
    for (idx_t t = 1; t <= max_t; ++t) {
        seq_t left  = _comb(m, t);
        seq_t right = _comb(upper, 5 * t);
        total += left * right;
    }

    return total;
}
