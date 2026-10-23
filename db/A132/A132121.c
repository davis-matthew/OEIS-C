/*
 * A132121  Triangle read by rows: T(n,k)=n*(n+1)*((3*k+2)*n+1)/6, 0<=k<=n.
 * https://oeis.org/A132121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: a(n) is the n‑th element of the triangle
   defined by T(r,k) = r*(r+1)*((3*k+2)*r+1)/6   (0 ≤ k ≤ r).
   The triangle is read row‑wise, so a(n) = T(r,k) where
   r is the unique integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
   and k = n – r*(r+1)/2.  The formula is evaluated safely
   by performing the two divisions (by 2 and 3) before the
   final multiplication to avoid overflow. */

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 <= n */
    idx_t r = 0;
    /* Increment r until the next row would exceed n */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* First index of this row */
    idx_t start = r * (r + 1) / 2;

    /* Column index within the row */
    idx_t k = n - start;

    /* Convert to seq_t for arithmetic */
    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;

    /* Compute T(r,k) = r*(r+1)*((3*k+2)*r+1)/6 */
    seq_t a_part = rn;            /* r   */
    seq_t b_part = rn + 1;        /* r+1 */
    /* Divide one of a_part or b_part by 2 (exact) */
    if (a_part % 2 == 0)
        a_part /= 2;
    else
        b_part /= 2;

    seq_t c_part = ((seq_t)3 * kn + (seq_t)2) * rn + (seq_t)1; /* ((3*k+2)*r+1) */

    /* Divide by 3, preferring to divide c_part when possible */
    if (c_part % 3 == 0)
        c_part /= 3;
    else if (a_part % 3 == 0)
        a_part /= 3;
    else
        b_part /= 3;

    return a_part * b_part * c_part;
}
