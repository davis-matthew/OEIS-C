/*
 * A056559  Tetrahedron with T(t,n,k) = t - n; succession of growing finite
 *          triangles with declining values per row.
 * https://oeis.org/A056559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A056556(n) - A056557(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the value t‑r where n lies in triangle t (rows 0…t) */
/* Each triangle t consists of (t+1)(t+2)/2 terms, and row r of triangle t has r+1 terms,
   all equal to t‑r.                           */
seq_t a(idx_t n)
{
    /* locate the triangle that contains index n */
    idx_t cumulative = 0;   /* number of terms before current triangle   */
    idx_t t = 0;            /* current triangle index                    */

    while (1) {
        /* size of triangle t  = (t+1)(t+2)/2   */
        idx_t size = (unsigned __int128)(t + 1) * (t + 2) >> 1;

        if (cumulative + size > n)
            break;          /* n is inside this triangle                */

        cumulative += size;
        ++t;
    }

    /* index inside triangle t */
    idx_t offset = n - cumulative;

    /* locate the row r inside triangle t */
    idx_t cumrow = 0;
    idx_t r = 0;

    while (1) {
        idx_t rowlen = r + 1;
        cumrow += rowlen;

        if (cumrow > offset)
            break;          /* offset lies in this row                   */

        ++r;
    }

    /* value in that row is t - r */
    return (seq_t)(t - r);
}
