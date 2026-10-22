/*
 * A003768  Number of spanning trees with degrees 1 and 3 in W_4 X P_n.
 * https://oeis.org/A003768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: 2*x*(1 -6*x +22*x^2 -52*x^3 +34*x^4 +92*x^5 -222*x^6
 *            +184*x^7 -24*x^8 -64*x^10) / (1 -14*x +62*x^2 -148*x^3 +264*x^4
 *            -336*x^5 +256*x^6 -128*x^7 +64*x^8). With b(m) = a(m+1)/2 and
 *            p(m) the numerator coefficients (p(m) = 0 for m > 10): b(m) =
 *            p(m) + 14*b(m-1) - 62*b(m-2) + 148*b(m-3) - 264*b(...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* numerator split into positive and negative parts */
    static const int ppos[11] = {1, 0, 22, 0, 34, 92, 0, 184, 0, 0, 0};
    static const int pneg[11] = {0, 6, 0, 52, 0, 0, 222, 0, 24, 0, 64};
    /* s[j] = b(m-1-j) */
    seq_t s[8];
    int j;
    idx_t m;
    seq_t v = 0;

    if (n < 1) return (seq_t)0;
    for (j = 0; j < 8; j++) s[j] = (seq_t)0;

    for (m = 0; m < n; m++) {
        seq_t pos = (seq_t)14 * s[0] + (seq_t)148 * s[2]
                  + (seq_t)336 * s[4] + (seq_t)128 * s[6];
        seq_t neg = (seq_t)62 * s[1] + (seq_t)264 * s[3]
                  + (seq_t)256 * s[5] + (seq_t)64 * s[7];
        if (m < 11) {
            pos += (seq_t)ppos[m];
            neg += (seq_t)pneg[m];
        }
        v = pos - neg;
        for (j = 7; j > 0; j--) s[j] = s[j - 1];
        s[0] = v;
    }
    return (seq_t)2 * v;
}
