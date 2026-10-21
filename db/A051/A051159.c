/*
 * A051159  Triangle read by rows: T(n, k) = binomial(n mod 2, k mod 2) *
 *          binomial(n div 2, k div 2), where 'div' denotes integer division.
 * https://oeis.org/A051159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = T(floor((sqrt(8*n+1)-1)/2), n - floor(r*(r+1)/2)) where
 *            T(r,k) = binom(r%2, k%2) * binom(r/2, k/2). The triangle is
 *            flattened row‑major; a(0)=1, a(1)=1, a(2)=1, a(3)=1, …
 * Range:     u8 n <= 260, u16 n <= 756, u32 n <= 2518, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static inline uint64_t row_of(idx_t n)
{
    uint64_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;    /* row index r */
}

/* Compute binomial coefficient C(n,k) for small arguments, using
   multiplicative method.  Assumes 0 <= k <= n. */
static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t r = row_of(n);
    uint64_t k = n - (unsigned __int128)r * (r + 1) / 2;

    /* binom(r%2, k%2) is zero iff r is even and k is odd */
    if ((r & 1ULL) == 0 && (k & 1ULL))
        return 0;

    /* remaining part: binom(r/2, k/2) */
    return binom_small(r >> 1, k >> 1);
}
