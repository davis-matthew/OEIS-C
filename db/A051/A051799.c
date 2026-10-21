/*
 * A051799  Partial sums of A007587.
 * https://oeis.org/A051799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(5*n+2)/12
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 317, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t v[4];
    v[0] = nseq + 1;
    v[1] = nseq + 2;
    v[2] = nseq + 3;
    v[3] = (5 * nseq) + 2;   /* (5*n+2) */

    /* Cancelling the factor 12 = 2*2*3 before the final multiplication
       prevents overflow on 64‑bit builds for all n used in the tests. */
    const int divs[3] = {2, 2, 3};
    for (int k = 0; k < 3; ++k) {
        int f = divs[k];
        for (int i = 0; i < 4; ++i) {
            if (v[i] % f == 0) {
                v[i] = v[i] / f;
                break;
            }
        }
    }

    seq_t res = v[0];
    res = res * v[1];
    res = res * v[2];
    res = res * v[3];
    return res;
}
