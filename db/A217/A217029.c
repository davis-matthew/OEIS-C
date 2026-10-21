/*
 * A217029  Array T(i,j) read by antidiagonals, where T(i,j) is the height of
 *          i/j, that is max(|m|,|n|) with m/n = i/j and gcd(m, n) = 1.
 * https://oeis.org/A217029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Height of reduced fraction corresponding to the n‑th entry of the
   array T(i,j) read by antidiagonals.  For the i‑th row and j‑th column
   the value is max(m,n) where m/n = i/j is written in lowest terms.  */

/* Computes gcd for idx_t values.  */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return a(n) for 1‑based index n.  */
seq_t a(idx_t n)
{
    /* Find the diagonal s such that (s-1)*s/2 >= n.
       Binary search works safely for all n that fit in uint64_t. */
    idx_t low = 1, high = n + 1;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val < (unsigned __int128)n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t s = low;

    /* Number of terms before diagonal s */
    idx_t prev = (s - 2) * (s - 1) / 2;
    /* Position within the diagonal (1‑based) */
    idx_t p = n - prev;

    /* Coordinates in the array: i increases along the diagonal. */
    idx_t i = p;
    idx_t j = s - p;

    /* Reduce fraction i/j and take the maximum of the two coprime
       numbers (the height). */
    idx_t g = gcd_idx(i, j);
    idx_t m = i / g;
    idx_t n2 = j / g;
    idx_t h = (m > n2) ? m : n2;

    return (seq_t)h;
}
