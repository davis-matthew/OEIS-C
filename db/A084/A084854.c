/*
 * A084854  Triangular array, read by rows: T(n,k) = concatenated decimal
 *          representations of n and k, 1<=k<=n.
 * https://oeis.org/A084854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = concatenate decimal of row r and position k where r is
 *            the smallest integer with r(r+1)/2 ≥ n, and k = n - r(r-1)/2
 * Range:     u8 n <= 54, u16 n <= 5049, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer square root of a 64‑bit unsigned integer. */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;          /* maximum possible sqrt(2^64‑1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;        /* upper mid */
        if (mid <= x / mid)                       /* avoid overflow: mid*mid ≤ x */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Return number of decimal digits in a positive integer. */
static int num_digits_u64(uint64_t v)
{
    int d = 0;
    while (v) {
        v /= 10;
        ++d;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* Guard against n == 0 (undefined for this sequence). */
    if ( n == 0 ) return (seq_t)0;

    uint64_t idx = (uint64_t)n;                    /* convert to native type */

    /* Find the row r such that r(r+1)/2 ≥ idx. */
    uint64_t r = (isqrt_u64((uint64_t)(8 * idx + 1)) - 1) >> 1;
    if ( r * (r + 1) / 2 < idx ) ++r;              /* adjust if necessary */

    /* Position k in that row. */
    uint64_t prev_rows = (r - 1) * r / 2;
    uint64_t k = idx - prev_rows;

    /* Compute 10^digits(k). */
    int dk = num_digits_u64(k);
    seq_t pow10 = (seq_t)1;
    for (int i = 0; i < dk; ++i) pow10 *= (seq_t)10;

    /* Concatenate r and k. */
    return (seq_t)r * pow10 + (seq_t)k;
}
