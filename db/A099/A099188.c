/*
 * A099188  a(n) = 2*ceiling(n/sqrt(2)).
 * https://oeis.org/A099188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ceiling(n / sqrt(2))
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^2 (fits in 64 bits for the tested range). */
    idx_t n_sq = n * n;

    /* Binary search for the smallest k with 2*k*k >= n^2. */
    idx_t low  = 0;
    idx_t high = n;          /* k cannot exceed n. */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t val = 2 * mid * mid;   /* 2*mid^2, safe for n ≤ 10 000. */

        if (val >= n_sq)
            high = mid;
        else
            low = mid + 1;
    }

    /* The result is 2*low. */
    return (seq_t)(2 * low);
}
