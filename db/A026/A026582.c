/*
 * A026582  a(n) = Sum{T(i,j)}, 0<=j<=i, 0<=i<=n, T given by A026568.
 * https://oeis.org/A026582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A026582
   a(n) = Sum_{0<=j<=i<=n} T(i,j),  T given by A026568
   Recurrence (for n>=6):
      a(n) = ( (4n-2) a(n-1) + (2n-6) a(n-2)
               - (20n-40) a(n-3) + (7n-6) a(n-4)
               + (24n-82) a(n-5) - (16n-56) a(n-6) ) / n
   Initial values: a(0)=1, a(1)=3, a(2)=8, a(3)=20, a(4)=50, a(5)=125
*/
seq_t a(idx_t n) {
    /* initial terms */
    const seq_t init[6] = { (seq_t)1, (seq_t)3, (seq_t)8,
                            (seq_t)20, (seq_t)50, (seq_t)125 };

    if (n < 6) {
        return init[(idx_t)n];
    }

    /* ring buffer for a(n-6)..a(n-1) */
    seq_t buf[6];
    for (int k = 0; k < 6; ++k) buf[k] = init[k];

    for (idx_t cur = 6; cur <= n; ++cur) {
        seq_t nseq = (seq_t)cur;

        /* coefficients */
        seq_t c1 = ((seq_t)4 * nseq) - ((seq_t)2);   /* 4n-2   */
        seq_t c2 = ((seq_t)2 * nseq) - ((seq_t)6);   /* 2n-6   */
        seq_t c3 = ((seq_t)20 * nseq) - ((seq_t)40); /* 20n-40 */
        seq_t c4 = ((seq_t)7 * nseq) - ((seq_t)6);   /* 7n-6   */
        seq_t c5 = ((seq_t)24 * nseq) - ((seq_t)82); /* 24n-82 */
        seq_t c6 = ((seq_t)16 * nseq) - ((seq_t)56); /* 16n-56 */

        /* compute terms */
        seq_t t1 = c1 * buf[5]; /* a(n-1) */
        seq_t t2 = c2 * buf[4]; /* a(n-2) */
        seq_t t3 = c3 * buf[3]; /* a(n-3) */
        seq_t t4 = c4 * buf[2]; /* a(n-4) */
        seq_t t5 = c5 * buf[1]; /* a(n-5) */
        seq_t t6 = c6 * buf[0]; /* a(n-6) */

        /* numerator = t1 + t2 + t4 + t5 - t3 - t6 */
        seq_t numerator = t1 + t2 + t4 + t5;
        numerator -= t3;
        numerator -= t6;

        seq_t next = numerator / nseq; /* a(n) */

        /* shift buffer */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5]; /* a(n) */
}
