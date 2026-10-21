/*
 * A094415  Triangle T read by rows: dot product <r,r-1,...,1> *
 *          <s+1,s+2,...,r,1,2,...,s>.
 * https://oeis.org/A094415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle T(n,k) = n*(n^2 +
 *            3*n*(1+k) + 2 – 3*k^2)/6 column–wise. For the linear index n
 *            (starting at 0) let row = largest integer r with r*(r+1)/2 ≤ n k
 *            = n – r*(r+1)/2 then a(n) = T(row+1 , k)
 * Range:     u8 n <= 45, u16 n <= 1851, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t and seq_t are typedefs provided by the build system. */
seq_t a(idx_t n)
{
    /* Convert the request to a 64‑bit value for internal calculations.  */
    uint64_t idx = (uint64_t)n;
    /* Find the triangle row r such that r*(r+1)/2 ≤ idx < (r+1)*(r+2)/2.  */
    uint64_t r = 0;
    while ( (unsigned __int128)(r + 1) * (r + 2) / 2 <= idx )
        ++r;

    /* Offset of the first element of this row.  */
    uint64_t base = (unsigned __int128)r * (r + 1) / 2;
    /* Position inside the row.  */
    uint64_t k = idx - base;

    /* The triangle formula uses 1‑based row index.  */
    uint64_t row = r + 1;

    /* Compute the triangle entry using 128‑bit arithmetic to avoid
       overflow in intermediate steps.  */
    unsigned __int128 temp = (unsigned __int128)row *
        ( (unsigned __int128)row * row
          + 3 * (unsigned __int128)row * (1 + k)
          + 2
          - 3 * (unsigned __int128)k * k );
    temp /= 6;

    return (seq_t)temp;
}
