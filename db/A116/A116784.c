/*
 * A116784  Number of permutations of length n which avoid the patterns 2314,
 *          3241, 4312.
 * https://oeis.org/A116784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A116784): 
   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73, a(6)=244, a(7)=782.
   For n≥8: a(n) = 10·a(n‑1) – 40·a(n‑2) + 81·a(n‑3) – 86·a(n‑4)
                   + 44·a(n‑5) – 8·a(n‑6). */
seq_t a(idx_t n)
{
    /* handle the first seven terms */
    if (n <= 6) {
        static const seq_t init[6] = { (seq_t)1, (seq_t)2, (seq_t)6,
                                      (seq_t)21, (seq_t)73, (seq_t)244 };
        return init[n - 1];
    }
    if (n == 7)
        return (seq_t)782;

    /* slide a window of the last six terms: last[0] = a(n‑6), ..., last[5] = a(n‑1) */
    seq_t last[6];
    last[0] = (seq_t)2;   /* a2 */
    last[1] = (seq_t)6;   /* a3 */
    last[2] = (seq_t)21;  /* a4 */
    last[3] = (seq_t)73;  /* a5 */
    last[4] = (seq_t)244; /* a6 */
    last[5] = (seq_t)782; /* a7 */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t sumPos = (seq_t)10 * last[5] + (seq_t)81 * last[3] + (seq_t)44 * last[1];
        seq_t sumNeg = (seq_t)40 * last[4] + (seq_t)86 * last[2] + (seq_t)8 * last[0];
        seq_t next = sumPos - sumNeg;

        /* shift the window */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = next;
    }

    return last[5];
}
