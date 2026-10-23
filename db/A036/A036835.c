/*
 * A036835  Schoenheim bound L_1(n,8,7).
 * https://oeis.org/A036835
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L_1(n,8,7) = ceil( (n/8) * ceil( ((n-1)/7) * ceil(
 *            ((n-2)/6) * ceil( ((n-3)/5) * ceil( ((n-4)/4) * ceil( ((n-5)/3)
 *            * ceil((n-6)/2) ) ) ) ) )
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Start with the base of the recursion: L_1(n-6,2,1) = ceil((n-6)/2) */
    seq_t val = ((seq_t)(n - 5)) / 2;      /* ceil((n-6)/2)  ----------------------------------------------------- */

    for (idx_t k = 3; k <= 8; ++k) {
        /* v = n + k - 8  (so that v runs: n-5, n-4, ..., n) */
        idx_t v = n + k - 8;
        seq_t num = val * (seq_t)v; /* v * previous value */
        /* ceil division by k: (num + k - 1) / k  --------------------------------------- */
        val = (num + k - 1) / k;
    }

    return val;
}
