/*
 * A141432  Triangle T(n,k) = (k+1)*(n-k-1) read by rows.
 * https://oeis.org/A141432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   a(n) = (k+1)*(r-k-1) where r is the row such that r(r+1)/2 ≥ n
 *            and k = n - r(r-1)/2 (1 ≤ k ≤ r)
 * Range:     i8 n <= 261, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A141432. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, but guard anyway */
        return (seq_t)0;

    /* Find the smallest r such that r(r+1)/2 ≥ n.  Binary search
       uses 128‑bit arithmetic for the product to avoid overflow. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        unsigned __int128 needed = (unsigned __int128)2 * n;
        if (prod >= needed)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of terms before row r: (r-1)r/2.  Calculate with 128‑bit. */
    unsigned __int128 before = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)before;    /* column index, 1 ≤ k ≤ r */

    /* Evaluate (k+1)*(r-k-1) in seq_t.  All arithmetic uses seq_t. */
    seq_t row = (seq_t)r;
    seq_t col = (seq_t)k;
    seq_t term = (col + 1) * (row - col - 1);
    return term;
}
