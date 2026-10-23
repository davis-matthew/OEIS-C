/*
 * A141616  Even terms in A120070.
 * https://oeis.org/A141616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) generated row‑by‑row from the irregular triangle A120070
 *            (no simple closed form). Each row r (r ≥ 3) has L = ⌊(r‑1)/2⌋
 *            entries. The first entry of row r is 4·⌊r²/4⌋, minus 4 if r is
 *            even. Successive entries decrease by 4·(2k) for odd rows and
 *            4·(2k+1) for even rows (k = 1,2,…)
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; harmless fallback */

    /* find the row r containing the n‑th term and its 1‑based position pos */
    idx_t r = 3;                    /* rows 1 and 2 have no even terms */
    idx_t cum = 0;                  /* cumulative count up to previous row */
    while (1) {
        idx_t L = (r - 1) >> 1;     /* floor((r-1)/2) */
        idx_t end = cum + L;        /* last index in row r */
        if (n <= end) break;        /* n belongs to row r */
        cum = end;
        ++r;
    }
    idx_t pos = n - cum;            /* 1‑based position within row r */

    /* compute the first entry of row r */
    idx_t parity = r & 1;           /* 1 if r odd, 0 if even */
    seq_t rr = (seq_t)r * (seq_t)r; /* r², cast to seq_t */
    seq_t base = rr / 4;            /* floor(r²/4)      */
    if (parity == 0) base -= 1;     /* even row: subtract 1 */
    seq_t val = base * 4;           /* first entry of row */

    /* apply successive differences for positions beyond the first */
    for (idx_t k = 1; k < pos; ++k) {
        seq_t step = 4 * (((seq_t)(2 * k)) + (parity == 0 ? 1 : 0));
        val -= step;
    }
    return val;
}
