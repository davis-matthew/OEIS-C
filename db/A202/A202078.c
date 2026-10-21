/*
 * A202078  Number of arrays of 7 integers in -n..n with sum zero and the sum
 *          of every adjacent pair being odd.
 * https://oeis.org/A202078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n−1) +6·a(n−2) −6·a(n−3) −15·a(n−4) +15·a(n−5)
 *            +20·a(n−6) −20·a(n−7) −15·a(n−8) +15·a(n−9) +6·a(n−10)
 *            −6·a(n−11) −a(n−12) +a(n−13) Initial values: a(1)=6, a(2)=96,
 *            a(3)=1014, a(4)=3752, a(5)=15010, a(6)=35604, a(7)=95342,
 *            a(8)=181834, a(9)=390544, a(10)=654086, a(11)=1221708,
 *            a(12)=18769...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 44, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init_terms[13] = {
    (seq_t)6,      (seq_t)96,     (seq_t)1014,
    (seq_t)3752,   (seq_t)15010,  (seq_t)35604,
    (seq_t)95342,  (seq_t)181834, (seq_t)390544,
    (seq_t)654086, (seq_t)1221708,(seq_t)1876930,
    (seq_t)3183404
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n<1, return 0 */
    if (n <= 13) return init_terms[n-1];

    /* sliding window of the previous 13 terms */
    seq_t last[13];
    for (int k = 0; k < 13; ++k) last[k] = init_terms[k];

    seq_t next = 0;
    for (idx_t i = 14; i <= n; ++i) {
        next = last[12]
             + ((seq_t)6) * last[11]
             - ((seq_t)6) * last[10]
             - ((seq_t)15) * last[9]
             + ((seq_t)15) * last[8]
             + ((seq_t)20) * last[7]
             - ((seq_t)20) * last[6]
             - ((seq_t)15) * last[5]
             + ((seq_t)15) * last[4]
             + ((seq_t)6) * last[3]
             - ((seq_t)6) * last[2]
             - last[1]
             + last[0];

        /* shift the window: drop last[0], push next into last[12] */
        for (int k = 0; k < 12; ++k) last[k] = last[k + 1];
        last[12] = next;
    }
    return next;
}
