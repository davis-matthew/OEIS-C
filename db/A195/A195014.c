/*
 * A195014  Vertex number of a square spiral whose edges have length A195013.
 * https://oeis.org/A195014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n^2 + 18*n + 3 + (2*n - 3)*(-1)^n) / 16
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the closed‑form expression; all intermediate arithmetic fits
   into seq_t for any n that fits the sequence type. */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t n_t    = (seq_t)n;                     // cast index to seq_t
    seq_t sum1   = (seq_t)10 * n_t * n_t + (seq_t)18 * n_t + (seq_t)3;
    seq_t term2  = (seq_t)2 * n_t;               // 2*n
    seq_t numerator;

    if ((n & 1) == 0) {
        /* even n : add (2*n - 3)  (positive for n>=2) */
        numerator = sum1 + (term2 - (seq_t)3);
    } else {                /* odd n */
        if (n == 1) {
            /* 2*n - 3 == -1, so we add 1 */
            numerator = sum1 + (seq_t)1;
        } else {
            /* 2*n - 3 > 0, subtract it */
            numerator = sum1 - (term2 - (seq_t)3);
        }
    }
    return numerator / (seq_t)16;
}
