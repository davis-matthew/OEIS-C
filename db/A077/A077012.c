/*
 * A077012  Triangle in which n-th row contains all possible products of n-1
 *          of the first n natural numbers in ascending order.
 * https://oeis.org/A077012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077012:
   a(n) = (r!)/k,
   where r = smallest integer such that r*(r+1)/2 >= n,
   and k = r - (n - r*(r-1)/2 - 1).
   This yields the n-th term of the triangle whose rows contain all
   products of r-1 of the first r natural numbers, sorted in increasing order. */

static idx_t triangular(idx_t k)
{
    return k * (k + 1) / 2;
}

/* Find the row number r that contains the n-th term. */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n = 0; return 0 for safety */

    /* Determine row r and position within that row. */
    idx_t r      = find_row(n);
    idx_t prev   = triangular(r - 1);
    idx_t pos    = n - prev - 1;           /* 0‑based index in this row */
    idx_t omitted = r - pos;               /* element omitted from the product */

    /* Compute factorial(r).  The value fits in seq_t for all terms that
       are required by the tests; for larger r a 128‑bit or arbitrary
       precision build will handle the overflow correctly. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= r; ++i)
        fact *= (seq_t)i;

    /* Since omitted divides r! exactly, the division truncates to
       the exact integer result. */
    return fact / (seq_t)omitted;
}
