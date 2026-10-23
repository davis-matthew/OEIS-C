/*
 * A127322  Second 4-dimensional hyper-tetrahedral coordinate; 4-D analog of
 *          A056557.
 * https://oeis.org/A127322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the second coordinate X in the lexicographic ordering *
 *            of all non‑negative 4‑tuples (W,X,Y,Z) with W≥X≥Y≥Z. * The
 *            number of tuples with first component W equals * C(W+3,3) =
 *            (W+3)(W+2)(W+1)/6 . * For a fixed W the number of tuples with a
 *            given X is * C(X+2,2) = (X+2)(X+1)/2 . * * To find...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t comb4(seq_t k) {          /* C(k,4) */
    if (k < 4) return 0;
    return k * (k - 1) * (k - 2) * (k - 3) / 24;
}

static inline seq_t comb3(seq_t k) {          /* C(k,3) */
    if (k < 3) return 0;
    return k * (k - 1) * (k - 2) / 6;
}

seq_t a(idx_t n) {
    /* Step 1: find the layer W */
    idx_t W = 0;
    while (1) {
        seq_t total = comb4((seq_t)(W + 4));
        if (total > n) break;
        W++;
    }

    /* Step 2: remaining offset inside the W‑layer */
    seq_t prev_total = comb4((seq_t)(W + 3));
    seq_t offset = (seq_t)n - prev_total;

    /* Step 3: locate X within this layer */
    idx_t X = 0;
    while (1) {
        seq_t cum = comb3((seq_t)(X + 3));
        if (offset < cum) break;
        X++;
    }

    return (seq_t)X;
}
