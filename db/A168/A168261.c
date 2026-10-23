/*
 * A168261  Triangle read by rows, A115361 * the diagonalized variant of
 *          A018819.
 * https://oeis.org/A168261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A018819(j) if the row‑column pair (r,j) of the linear
 *            index n satisfies r % j == 0 and (r/j) is a power of two,
 *            otherwise a(n) = 0. Here A018819(j) = 1,1,2,2,4,4,6,6,10,10,...
 *            1‑based indices
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for non‑positive indices */

    /* Find the row r such that the first index of that row is <= n
       and the first index of the next row is > n.  Use the running
       triangular sum T(r) = r(r+1)/2. */
    idx_t r = 1;
    idx_t tri = 1;                       /* T(1) */
    while (tri < n) {
        ++r;
        tri += r;                        /* now tri = T(r) */
    }

    /* First index of row r */
    idx_t start = tri - r + 1;           /* offset_row(r) */
    idx_t j = n - start + 1;             /* column (1‑based) */

    /* Entry in triangle M is non‑zero only if r/j is a power of two. */
    if (r % j != 0) return 0;
    idx_t div = r / j;
    if ((div & (div - 1)) != 0) return 0;

    /* Compute A018819(j).  The values are repeated twice:
       q_{2k-1} = q_{2k} = g(k), with g(1)=1,g(2)=2,g(3)=4,g(4)=6,...   */
    idx_t k = (j + 1) / 2;                /* k = ceil(j/2) */

    if (k == 1) return (seq_t)1;          /* g(1) = 1 */

    if ((k & 1) == 0) {                  /* k even -> g(k) = 2m^2 - 2m + 2 */
        idx_t m = k / 2;
        seq_t mm = (seq_t)m;
        return (seq_t)(2 * mm * mm - 2 * mm + 2);
    } else {                              /* k odd > 1 -> g(k) = 2(m-1)^2 + 2 */
        idx_t m = (k + 1) / 2;            /* m = (k+1)/2 */
        seq_t mm = (seq_t)m;
        return (seq_t)(2 * (mm - 1) * (mm - 1) + 2);
    }
}
