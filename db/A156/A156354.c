/*
 * A156354  Triangle T(n, k) = k^(n-k) + (n-k)^k with T(0, 0) = 1, read by
 *          rows.
 * https://oeis.org/A156354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) read by rows, where * T(r,k) = k^(r-k) + (r-k)^k,
 *            T(0,0) = 1
 * Range:     u8 n <= 38, u16 n <= 81, u32 n <= 194, u64 n <= 471,
 *            u128 n <= 495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq_custom(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;    /* T(0,0) */

    /* Find the row r such that offset(r) = r*(r+1)/2 <= n < offset(r+1) */
    idx_t low = 0;
    idx_t high = n;             /* r cannot exceed n */
    while (low < high)
    {
        idx_t mid = low + ((high - low + 1) >> 1);
        seq_t off_mid = ((seq_t)mid * (mid + 1)) / 2;
        if (off_mid <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r = low;
    seq_t off_r = ((seq_t)r * (r + 1)) / 2;
    idx_t offset = (idx_t)off_r;      /* safe cast: offset fits in idx_t */
    idx_t k = n - offset;             /* column index within row r */

    /* Compute T(r,k) = k^(r-k) + (r-k)^k */
    seq_t term1 = pow_seq_custom((seq_t)k, r - k);            /* k^(r-k) */
    seq_t term2 = pow_seq_custom((seq_t)(r - k), k);          /* (r-k)^k */

    return term1 + term2;
}
