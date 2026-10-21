/*
 * A094310  Triangle read by rows: T(n,k), the k-th term of the n-th row, is
 *          the product of all numbers from 1 to n except k: T(n,k) = n!/k.
 * https://oeis.org/A094310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = factorial(r)/c, where r is the smallest integer such *
 *            that r(r+1)/2 >= n and c = n - (r-1)r/2. This implements the
 *            OEIS * triangle T(n,k)=n!/k read by rows
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 78, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r by binary search on r*(r+1)/2 >= n. */
    idx_t low = 1, high = n;          /* high can't exceed n */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        idx_t sum = mid * (mid + 1) / 2;   /* fits in 64‑bit for n <= 10k */
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index within the row: c = n - sum(rows 1..r-1) */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t c = n - prev_sum;           /* 1 <= c <= r */

    /* Compute r! in seq_t */
    seq_t fact = 1;
    for (idx_t i = 2; i <= r; ++i) {
        fact *= (seq_t)i;
    }

    /* Return T(r,c) = (r!)/c */
    return fact / (seq_t)c;
}
