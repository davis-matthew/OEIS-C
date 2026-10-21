/*
 * A054130  T(n,4), array T as in A054126.
 * https://oeis.org/A054130
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n,4) as defined in OEIS A054130. Recurrence (for n ≥
 *            14): a(n) = 10*a(n-1) - 44*a(n-2) + 112*a(n-3) - 182*a(n-4) +
 *            196*a(n-5) - 140*a(n-6) + 64*a(n-7) - 17*a(n-8) + 2*a(n-9) Base
 *            values for 4 ≤ n ≤ 13 are taken from the sequence definition
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    static const seq_t base[14] = {
        0, 0, 0, 0,          /* indices 0..3 (unused) */
        2,          /* a(4) */
        11,         /* a(5) */
        58,         /* a(6) */
        236,        /* a(7) */
        802,        /* a(8) */
        2396,       /* a(9) */
        6508,       /* a(10) */
        16448,      /* a(11) */
        39331,      /* a(12) */
        90102       /* a(13) */
    };

    if (n <= 13) return base[n];

    /* buffer holding the last nine computed terms:
       seq[0] = a(curr-9), seq[8] = a(curr-1)                   */
    seq_t seq[9];

    seq[0] = (seq_t)11;   /* a(5) */
    seq[1] = (seq_t)58;   /* a(6) */
    seq[2] = (seq_t)236;  /* a(7) */
    seq[3] = (seq_t)802;  /* a(8) */
    seq[4] = (seq_t)2396; /* a(9) */
    seq[5] = (seq_t)6508; /* a(10) */
    seq[6] = (seq_t)16448;/* a(11) */
    seq[7] = (seq_t)39331;/* a(12) */
    seq[8] = (seq_t)90102;/* a(13) */

    seq_t next = 0;
    for (idx_t curr = 14; curr <= n; ++curr) {
        next = 10 * seq[8]          /* a(curr-1)   */
             - 44 * seq[7]          /* a(curr-2)   */
             + 112 * seq[6]         /* a(curr-3)   */
             - 182 * seq[5]         /* a(curr-4)   */
             + 196 * seq[4]         /* a(curr-5)   */
             - 140 * seq[3]         /* a(curr-6)   */
             + 64  * seq[2]         /* a(curr-7)   */
             - 17  * seq[1]         /* a(curr-8)   */
             + 2   * seq[0];        /* a(curr-9)   */

        /* shift buffer for next iteration */
        seq[0] = seq[1];
        seq[1] = seq[2];
        seq[2] = seq[3];
        seq[3] = seq[4];
        seq[4] = seq[5];
        seq[5] = seq[6];
        seq[6] = seq[7];
        seq[7] = seq[8];
        seq[8] = next;
    }
    return next;
}
