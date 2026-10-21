/*
 * A089083  T(n,k) = (floor(k*n/2) * ceiling(k*n/2))^2, triangular array read
 *          by rows, 1 <= k <= n.
 * https://oeis.org/A089083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ( floor(k * r / 2) * ceil(k * r / 2) )^2, where r is the
 *            row number (1‑based) and k is the column (1‑based) such that the
 *            linear index n lies in row r and column k
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 275, u64 n <= 9640,
 *            u128 n <= 9640, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset: the sequence starts at a(1).  Return 0 for n==0. */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r so that r(r+1)/2 >= n > (r-1)r/2. */
    idx_t low = 1;
    idx_t high = n;          /* r cannot exceed n */
    idx_t r = 0;

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t t = mid * (mid + 1) / 2;   /* number of terms up to row mid */
        if (t >= n) {
            r = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* Compute the column index k (1‑based). */
    idx_t prev = r * (r - 1) / 2;   /* terms before row r */
    idx_t k = n - prev;            /* 1 ≤ k ≤ r */

    /* Apply the formula using the row number r, not the linear index n. */
    seq_t kn = (seq_t)k * (seq_t)r;      /* integer multiplication */
    seq_t floor_kn = kn / 2;
    seq_t ceil_kn  = (kn + 1) / 2;       /* ceil(k*r/2) */
    seq_t prod = floor_kn * ceil_kn;     /* product of floor and ceil */
    return prod * prod;                  /* square of the product */
}
