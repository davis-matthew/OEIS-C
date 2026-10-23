/*
 * A169048  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=35, a(2)=1190, ..., a(23) as computed below. For n
 *            ≥ 24 a(n) satisfies the linear recurrence a(n) =
 *            33·(a(n-1)+a(n-2)+…+a(n-23)) – 561·a(n-24) These coefficients
 *            come from the generating function (1 + 2t + 2t² + … + 2t²³ +
 *            t²⁴) -------------------------------------- (1 – 33t – 33t² –...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute first 24 terms using the generating function */
    seq_t pre[24];
    seq_t sum = 0;
    for (idx_t i = 0; i < 24; ++i) {
        seq_t p = (i == 0) ? (seq_t)1 : (seq_t)2;   /* numerator coeffs */
        pre[i] = p + (seq_t)33 * sum;                /* recurrence for n<24 */
        sum += pre[i];
    }

    if (n < 24)
        return pre[n];

    /* for n ≥ 24 use the 24‑term linear recurrence */
    seq_t sum23 = 0;
    for (idx_t i = 1; i < 24; ++i)   /* sum of a[1]..a[23] */
        sum23 += pre[i];

    for (idx_t cur = 24; cur <= n; ++cur) {
        seq_t next = (seq_t)33 * sum23 - (seq_t)561 * pre[(cur - 24) % 24];
        pre[cur % 24] = next;     /* store a[cur] */
        sum23 += next;            /* update sum of last 23 terms */
        sum23 -= pre[(cur - 23) % 24];
    }
    return pre[n % 24];
}
