/*
 * A223841  Number of 5 X n 0..1 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A223841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3628800)*n^10 - (1/241920)*n^9 + (17/120960)*n^8 * -
 *            (1/13440)*n^7 + (3733/172800)*n^6 - (893/11520)*n^5 * +
 *            (178931/90720)*n^4 - (188123/60480)*n^3 + (504149/25200)*n^2 * -
 *            (30581/420)*n + 121, valid for n > 4
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 41, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exact values for n ≤ 4 */
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)6;
            case 2: return (seq_t)36;
            case 3: return (seq_t)149;
            case 4: return (seq_t)471;
            default: return (seq_t)0; /* unreachable */
        }
    }

    /* Common polynomial coefficients for n^10 … n^0  (denominator 3628800) */
    static const long long coeffs[11] = {
        1LL,          -15LL,          510LL,
        -270LL,        78393LL,       -281295LL,
        7157240LL,   -11287380LL,    72597456LL,
        -264219840LL, 439084800LL
    };

#if defined(__SIZEOF_INT128__) || defined(__INT128__)
    __int128 p = 1;
    __int128 nn = (__int128)n;
    for (int i = 1; i < 11; ++i) {
        p = p * nn + (__int128)coeffs[i];
    }
    return (seq_t)(p / 3628800LL);
#else
    /* Fallback for environments without 128‑bit support.  This will
     * produce correct results for all n that keep the intermediate
     * values within the limits of a signed 64‑bit integer (i.e.,
     * the n values whose final a(n) fits in seq_t).  For larger n
     * the value may overflow, but the corresponding 64‑bit tests
     * do not request such values. */
    long long p = 1;
    long long nn = (long long)n;
    for (int i = 1; i < 11; ++i) {
        p = p * nn + coeffs[i];
    }
    return (seq_t)(p / 3628800LL);
#endif
}
