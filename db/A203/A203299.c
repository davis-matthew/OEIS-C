/*
 * A203299  Second elementary symmetric function of the first n terms of
 *          (2,2,3,3,4,4,5,5...).
 * https://oeis.org/A203299
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( (∑_{k=1}^n a_k)^2 - ∑_{k=1}^n a_k² ) / 2, where the
 *            base sequence a_k = ⌈k/2⌉ + 1 (i.e. 2,2,3,3,4,4,…)
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset: a(2) is the first term, defined only for n >= 2 */
    if (n < 2) return 0;   /* undefined for offset values, return 0 */

    /* number of full pairs among the first n terms */
    idx_t pairs = n >> 1;             /* floor(n/2)   */
    seq_t M = (seq_t)pairs;           /* integer of type seq_t */

    seq_t S1;      /* sum_{k=1}^n a_k   */
    seq_t S2;      /* sum_{k=1}^n a_k²  */

    if ((n & 1) == 0) {  /* even n = 2M */
        /* S1 = M*(M+3) */
        S1 = M * (M + (seq_t)3);

        /* S2 = 2 * ( (M+1)(M+2)(2M+3)/6 - 1 ) */
        seq_t A = M + (seq_t)1;                          /* M+1         */
        seq_t B = M + (seq_t)2;                          /* M+2         */
        seq_t C = (seq_t)2 * A + (seq_t)1;               /* 2M+3        */
        seq_t prod = A * B;
        prod = prod * C;                                /* (M+1)(M+2)(2M+3) */
        seq_t sum_u2 = prod / (seq_t)6;                 /* (M+1)(M+2)(2M+3)/6 */
        seq_t sum_t1sq = sum_u2 - (seq_t)1;             /* ∑_{u=2}^{M+1} u² */
        S2 = (seq_t)2 * sum_t1sq;                       /* 2 * sum_t1sq */
    } else {               /* odd  n = 2M+1 */
        /* S1 = M*(M+3) + (M+2) */
        S1 = M * (M + (seq_t)3) + (M + (seq_t)2);

        /* S2 = 2 * ((M+1)(M+2)(2M+3)/6 - 1) + (M+2)² */
        seq_t A = M + (seq_t)1;                          /* M+1         */
        seq_t B = M + (seq_t)2;                          /* M+2         */
        seq_t C = (seq_t)2 * A + (seq_t)1;               /* 2M+3        */
        seq_t prod = A * B;
        prod = prod * C;                                /* (M+1)(M+2)(2M+3) */
        seq_t sum_u2 = prod / (seq_t)6;                 /* (M+1)(M+2)(2M+3)/6 */
        seq_t sum_t1sq = sum_u2 - (seq_t)1;             /* ∑_{u=2}^{M+1} u² */
        seq_t S2_pairs = (seq_t)2 * sum_t1sq;           /* 2 * sum_t1sq */
        seq_t extra = M + (seq_t)2;                     /* M+2         */
        S2 = S2_pairs + extra * extra;                 /* + (M+2)²    */
    }

    /* a(n) = (S1² - S2) / 2 */
    return (S1 * S1 - S2) / (seq_t)2;
}
