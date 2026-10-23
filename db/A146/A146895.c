/*
 * A146895  Weight distribution of [76,38,12] double circulant code.
 * https://oeis.org/A146895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the weight distribution of the [76,38,12] double
 *            circulant code. The codewords have even weight, so a(n)=0 for
 *            odd n. For 0 ≤ n ≤ 38 the values are symmetric: a(n)=a(38−n).
 *            The non‑zero coefficients are fixed: a(0)=1, a(6)=2109,
 *            a(8)=86469, a(9)=961704, a(10)=7489059, a(11)=53574224, a(12...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 14, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _wdlc_[20] = {
    (seq_t)1,
    (seq_t)0,
    (seq_t)0,
    (seq_t)0,
    (seq_t)0,
    (seq_t)0,
    (seq_t)2109,
    (seq_t)0,
    (seq_t)86469,
    (seq_t)961704,
    (seq_t)7489059,
    (seq_t)53574224,
    (seq_t)275509215,
    (seq_t)1113906312,
    (seq_t)3626095793,
    (seq_t)9404812736ULL,
    (seq_t)19610283420ULL,
    (seq_t)33067534032ULL,
    (seq_t)45200010670ULL,
    (seq_t)50157375456ULL
};

seq_t a(idx_t n) {
    if (n > 38u)              /* terms beyond length 76 are zero */
        return (seq_t)0;
    /* For 0 ≤ n ≤ 38, use symmetry a(n) = a(38−n). */
    return _wdlc_[ n <= 19u ? n : (38u - n) ];
}
