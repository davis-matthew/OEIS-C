/*
 * A104793  Triangle T(n,k) = A023537(n-k), n >= 1, 0 <= k < n, read by rows.
 * https://oeis.org/A104793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Lucas(r-k+4) - (3*r - 3*k + 7), where r = smallest
 *            integer with r(r+1)/2 ≥ n, and k = n - (r-1)r/2 - 1 (0‑based
 *            position in row r)
 * Range:     u8 n <= 28, u16 n <= 190, u32 n <= 903, u64 n <= 3916,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Precompute Lucas numbers (L(0)=2, L(1)=1).  Size is sufficient for n up to ~10⁴. */
static seq_t lucas[2000];
static idx_t lucas_len = 0;

/* Ensure lucas array contains values up to index 'up' (inclusive). */
static void init_lucas_up_to(idx_t up)
{
    if (up + 1 <= lucas_len)
        return;

    if (lucas_len == 0) {
        lucas[0] = (seq_t)2;
        lucas[1] = (seq_t)1;
        lucas_len = 2;
    }
    for (idx_t i = lucas_len; i <= up; ++i) {
        lucas[i] = lucas[i-1] + lucas[i-2];
    }
    lucas_len = up + 1;
}

/* Find the row index r (r ≥ 1) such that r(r+1)/2 ≥ n. */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Sequence function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset is 1, a(0) is unused */

    /* Locate row r and column k (0‑based) for index n. */
    idx_t r = find_row(n);
    __uint128_t tri_prev = (__uint128_t)(r - 1) * r / 2;   /* (r-1)r/2 */
    idx_t k = (idx_t)(n - (idx_t)tri_prev - 1);           /* 0 ≤ k < r */

    /* Index inside A023537 (and Lucas).  m = r - k. */
    idx_t m = r - k;
    idx_t lucas_idx = m + 4;      /* n-k+4 = r-k+4 = m+4 */
    init_lucas_up_to(lucas_idx);

    /* Compute the subtraction term: 3*r - 3*k + 7. */
    seq_t sub = (seq_t)3 * (seq_t)r - (seq_t)3 * (seq_t)k + (seq_t)7;

    return lucas[lucas_idx] - sub;
}
