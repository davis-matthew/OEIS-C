/*
 * A095851  Triangle read by rows: T(n,k) = (n-k+1)^((n-k+1)^2), n>=1,
 *          1<=k<=n.
 * https://oeis.org/A095851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (base)^(base*base) where * base = r - k + 1, * r =
 *            smallest integer with r*(r+1)/2 >= n (row number), * k = n -
 *            (r-1)*r/2 (column position in that row). * The triangle is read
 *            row‑by‑row
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t row_for_index(idx_t n)
{
    /* Binary search to find the smallest r such that r*(r+1)/2 >= n. */
    idx_t low = 1, high = 1;
    /* Grow high until it is large enough. Use __int128 to avoid overflow. */
    while ((__int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 total = (__int128)mid * (mid + 1) / 2;
        if (total >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k. */
    idx_t r = row_for_index(n);
    idx_t prev_terms = (__int128)(r - 1) * r / 2;  /* terms before row r */
    idx_t k = n - prev_terms;                    /* 1‑based column */
    idx_t base = r - k + 1;                       /* base of the power */

    idx_t exp = base * base;          /* exponent = base^2, fits in idx_t */
    return pow_seq_t((seq_t)base, exp);
}
