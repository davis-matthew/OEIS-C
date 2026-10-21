/*
 * A059173  Maximal number of regions into which 4-space can be divided by n
 *          hyperspheres.
 * https://oeis.org/A059173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + n·(n−1) + n·(n−1)·(n−2)·(n−3)/12 (for n ≥ 0)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 476, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 1      (the −1 from the e.g.f is already absorbed)          */
seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t term = 2;                      /* constant part (from 2·e^x) */
    if (n >= 2) {
        seq_t t = (seq_t)n * (seq_t)(n - 1); /* n·(n−1)   */
        term += t;
    }
    if (n >= 4) {
        /* compute n·(n−1)·(n−2)·(n−3) / 12  , all done in seq_t */
        seq_t t = (seq_t)n * (seq_t)(n - 1);
        t *= (seq_t)(n - 2);
        t *= (seq_t)(n - 3);
        t /= 12;
        term += t;
    }
    return term;
}
