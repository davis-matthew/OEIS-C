/*
 * A124171  Sequence obtained by reading the triangles shown below by rows.
 * https://oeis.org/A124171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = the offset of the n‑th element in the triangle whose row
 *            r contains the first triangular number T_r = r(r+1)/2 positive
 *            integers. The cumulative total of elements up to row r is the
 *            tetrahedral number Tetra(r) = r(r+1)(r+2)/6. To find a(n) we
 *            locate the row r with Tetra(r-1) < n ≤ Tetra(r)...
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t tetrahedral(idx_t r)
{
    seq_t rr = (seq_t)r;
    return rr * (rr + 1) * (rr + 2) / 6;
}

seq_t a(idx_t n)
{
    /* Any n ≥ 1 is valid; behaviour for n = 0 is undefined. */
    idx_t low  = 1;
    idx_t high = 1;
    seq_t sn   = (seq_t)n;

    /* Find an upper bound for the row index r. */
    while (tetrahedral(high) < sn)
    {
        /* Prevent overflow of high (idx_t is unsigned). */
        if (high > (idx_t)-2) { high = (idx_t)-1; break; }
        high <<= 1;
    }

    /* Binary search for the smallest r with tetrahedral(r) ≥ n. */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (tetrahedral(mid) < sn)
            low = mid + 1;
        else
            high = mid;
    }

    /* low (or high) is the desired row r. */
    idx_t r = low;
    seq_t prev = tetrahedral(r - 1);   /* Tetrahedral(r-1) */
    return sn - prev;                  /* Offset in row r */
}
