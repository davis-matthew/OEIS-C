/*
 * A176631  Triangle T(n, k) = 22^(k*(n-k)), read by rows.
 * https://oeis.org/A176631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 22^(k*(r-k)), where * r = floor( (sqrt(8*n+1)-1)/2 ) (row
 *            index, 0‑based) * k = n - r*(r+1)/2 (position in the row,
 *            0‑based) * if k==0 or k==r then a(n)=1 * else a(n)=22^[k*(r-k)]
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 38, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

static idx_t row_of(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Find an upper bound for r */
    while (high * (high + 1) / 2 <= n)
        high <<= 1;
    /* Binary search for largest r with r*(r+1)/2 <= n */
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t start = r * (r + 1) / 2;  /* index where row r starts */
    idx_t k  = n - start;

    if (k == 0 || k == r)
        return 1;

    idx_t exp = k * (r - k);
    return pow_seq(22, exp);
}
