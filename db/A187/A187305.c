/*
 * A187305  Number of 10-step one space leftwards or up, two space rightwards
 *          or down asymmetric rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 124248*n^2 - 1238146*n + 2807812 for n >= 18; for n < 18
 *            the values are taken from the first 17 terms of the sequence
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 18) {
        static const seq_t tbl[18] = {
            0ULL,          /* a(0) unused */
            0ULL,          /* a(1) */
            0ULL,          /* a(2) */
            0ULL,          /* a(3) */
            3096ULL,       /* a(4) */
            44824ULL,      /* a(5) */
            187924ULL,     /* a(6) */
            505878ULL,     /* a(7) */
            1044900ULL,    /* a(8) */
            1847506ULL,    /* a(9) */
            2912204ULL,    /* a(10) */
            4254568ULL,    /* a(11) */
            5853664ULL,    /* a(12) */
            7715596ULL,    /* a(13) */
            9827696ULL,    /* a(14) */
            12192058ULL,   /* a(15) */
            14805028ULL,   /* a(16) */
            17667036ULL    /* a(17) */
        };
        return tbl[n];
    }
    seq_t x = (seq_t)n;
    return 124248 * x * x - 1238146 * x + 2807812;
}
