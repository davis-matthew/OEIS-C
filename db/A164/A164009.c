/*
 * A164009  Zero together with row 9 of the array in A163280.
 * https://oeis.org/A164009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + 8n for n ≥ 13, A164009 = A028566(n) for n > 12
 *            (earlier values are explicitly specified)
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≤ 12 the values are provided explicitly.  They are
     * the first terms of the sequence as listed in OEIS A164009. */
    if (n <= 12) {
        static const seq_t early[13] = {
            0,   /* a(0) */
            19,  /* a(1) */
            38,  /* a(2) */
            51,  /* a(3) */
            76,  /* a(4) */
            75,  /* a(5) */
            114, /* a(6) */
            105, /* a(7) */
            136, /* a(8) */
            162, /* a(9) */
            190, /* a(10) */
            209, /* a(11) */
            264  /* a(12) */
        };
        return early[n];
    }

    /* For n ≥ 13 the sequence equals n^2 + 8n, which is the same as
     * the values of A028566(n) (conjectured for n > 12). */
    seq_t nn = (seq_t)n;
    return nn * (nn + (seq_t)8);
}
