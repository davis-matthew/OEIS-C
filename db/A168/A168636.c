/*
 * A168636  a(n) = n^7*(n^2 + 1)/2.
 * https://oeis.org/A168636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 149, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Formula: a(n) = n^7*(n^2 + 1)/2
 * The implementation pulls out the factor 2 early to avoid overflow
 * on 64‑bit builds for all indices whose exact value fits into seq_t.
 * It uses only seq_t arithmetic and standard operations.
 */
seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;

    /* Even n: pull out factor 2 from n^7 and multiply by 64 at the end */
    if ((n & 1ULL) == 0) {
        seq_t k = nn >> 1;                       /* k = n/2                */
        seq_t k2 = k * k;                        /* k^2                    */
        seq_t k4 = k2 * k2;                      /* k^4                    */
        seq_t k7 = k4 * k2 * k;                  /* k^7                    */

        seq_t n2 = nn * nn;                     /* n^2                    */
        seq_t prod = k7 * (n2 + (seq_t)1);       /* k^7 * (n^2 + 1)        */
        return prod << 6u;                       /* multiply by 64 (=2^6)   */
    }

    /* Odd n: divide (n^2+1) by 2 first */
    seq_t n2 = nn * nn;                          /* n^2                    */
    seq_t n2p1_over2 = (n2 + (seq_t)1) >> 1;      /* (n^2+1)/2              */

    seq_t n3 = nn * n2;                          /* n^3                    */
    seq_t n4 = n3 * nn;                          /* n^4                    */
    seq_t n7 = n4 * n3;                          /* n^7 = n^4 * n^3        */

    return n7 * n2p1_over2;                      /* final result           */
}
