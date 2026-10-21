/*
 * A077385  Triangle read by rows in which n-th row contains 2n-1 terms
 *          starting from n^0 to n^(n-1) in increasing order and then in
 *          decreasing order to n^0.
 * https://oeis.org/A077385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = r^e where r = ceil(sqrt(n)), e = k if k < r else 2r - k -
 *            2, k = n - (r-1)^2 - 1
 * Range:     u8 n <= 20, u16 n <= 42, u32 n <= 110, u64 n <= 272,
 *            u128 n <= 756, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer floor(sqrt(x)) */
static inline idx_t isqrt(idx_t x)
{
    idx_t low = 0, high = x, res = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0)
        {
            res = 0;
            low = 1;
            continue;
        }
        if (mid <= x / mid)
        {
            res = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return res;
}

/* Integer power: base^exp */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* Sequence A077385 */
seq_t a(idx_t n)
{
    /* n < 1 is out of specification; return 0 */
    if (n == 0)
        return (seq_t)0;

    /* Determine row r = ceil(sqrt(n)) = floor(sqrt(n-1)) + 1 */
    idx_t r = isqrt(n - 1) + 1;

    /* Position inside the row (0‑based) */
    idx_t pos = n - (r - 1) * (r - 1) - 1;

    /* Exponent in the power */
    idx_t exp = (pos < r) ? pos : 2 * r - pos - 2;

    seq_t base = (seq_t)r;
    return pow_seq(base, exp);
}
