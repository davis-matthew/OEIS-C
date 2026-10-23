/*
 * A130277  Triangle read by rows: T(n, k) = A000005(n) if k divides n, T(n,
 *          k) = 0 otherwise.
 * https://oeis.org/A130277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A000005(row) if (row % column == 0) else 0, where
 *            (row,column) are the coordinates of n in the triangle read by
 *            rows
 * Range:     u8 n <= 191, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r containing the nth triangle entry (1‑based). */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Grow high until the triangular number exceeds n */
    while (((seq_t)high * (high + 1)) / 2 < n)
        high <<= 1;
    /* Binary search for the smallest row with triangular(n) ≥ n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t tri = ((seq_t)mid * (mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Return the divisor count A000005(r). */
static seq_t divisor_count(idx_t r)
{
    seq_t d = 1;
    idx_t temp = r;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            d *= (seq_t)(e + 1);
        }
    }
    if (temp > 1)
        d *= 2;
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safe */

    idx_t r = find_row(n);

    /* triangular number preceding row r: (r-1)*r/2 */
    seq_t prev_tri = ((seq_t)(r - 1) * r) / 2;

    /* column index k (1‑based) */
    idx_t k = (idx_t)((seq_t)n - prev_tri);

    /* According to the definition, the entry is d(r) iff r is divisible by k */
    if (r % k == 0)
        return divisor_count(r);
    return 0;
}
