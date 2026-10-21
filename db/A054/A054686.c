/*
 * A054686  Multiset consisting of squares and triangular numbers.
 * https://oeis.org/A054686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 617, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiset of squares and triangular numbers (numbers belonging to both sets appear twice). */
/* a(n) is the n‑th smallest element in that multiset for n >= 1.  */
seq_t a(idx_t n)
{
    /* safety: undefined for n == 0 */
    if (n == 0)
        return (seq_t)0;

    idx_t i = 0;          /* index for squares (k^2) */
    idx_t j = 0;          /* index for triangular numbers (k(k+1)/2) */
    idx_t count = 0;      /* how many terms have been produced so far */

    while (1) {
        /* current candidates */
        seq_t sq  = (seq_t)i * (seq_t)i;                 /* k^2 */
        seq_t tri = (seq_t)j * (seq_t)(j + 1) / (seq_t)2; /* k(k+1)/2 */

        if (sq < tri) {
            count++;
            if (count == n)
                return sq;
            ++i;
        } else if (sq > tri) {
            count++;
            if (count == n)
                return tri;
            ++j;
        } else {  /* sq == tri : the number is both a square and a triangular */
            /* first occurrence (from the square side) */
            count++;
            if (count == n)
                return sq;
            /* second occurrence (from the triangular side) */
            count++;
            if (count == n)
                return tri;
            ++i;
            ++j;
        }
    }
}
