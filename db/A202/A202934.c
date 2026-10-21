/*
 * A202934  Number of (n+3) X 6 binary arrays with consecutive windows of four
 *          bits considered as a binary number nondecreasing in every row and
 *          column.
 * https://oeis.org/A202934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) – 21*a(n-2) + 35*a(n-3) – 35*a(n-4) * +
 *            21*a(n-5) – 7*a(n-6) + a(n-7) (n > 7) * with a(1)=104976,
 *            a(2)=153874, a(3)=225858, * a(4)=330853, a(5)=481798,
 *            a(6)=695114, a(7)=991196. * * The first seven terms are
 *            generated from the coefficient * list of the numerator of the
 *            generating...
 * Range:     u8 none, u16 none, u32 n <= 57, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Coefficients of the numerator:  C0+ C1*x + ... + C6*x^6 */
static const int64_t C[7] = {
    104976,          /* C0 */
   -580958,          /* C1 */
    1353236,         /* C2 */
   -1692959,         /* C3 */
    1197415,         /* C4 */
   -453495,          /* C5 */
     71809          /* C6 */
};

/* Computes binom(n,6) for n >= 6 using only seq_t arithmetic. */
static inline seq_t binom6(idx_t n)
{
    if (n < 6) return 0;
    seq_t r = 1;
    for (int i = 1; i <= 6; ++i) {
        r = (r * (seq_t)(n - 6 + i)) / i;
    }
    return r;
}

/* Sequence A202934. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence defined for n >= 1 */

    /* Initial values: compute once. */
    static seq_t init[7];
    static int init_done = 0;
    if (!init_done) {
        for (idx_t i = 1; i <= 7; ++i) {
            seq_t sum = 0;
            for (int j = 0; j < 7; ++j) {
                idx_t m = i - j + 5;
                if (m < 6) continue;
                sum += (seq_t)C[j] * binom6(m);
            }
            init[i - 1] = sum;
        }
        init_done = 1;
    }

    if (n <= 7) return init[n - 1];

    /* Apply recurrence for n >= 8. */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i) buf[i] = init[i];   /* a(n-7)...a(n-1) */

    for (idx_t m = 8; m <= n; ++m) {
        seq_t val = 7 * buf[6] - 21 * buf[5] + 35 * buf[4]
                  - 35 * buf[3] + 21 * buf[2] - 7 * buf[1] + buf[0];
        /* shift window */
        for (int i = 0; i < 6; ++i) buf[i] = buf[i + 1];
        buf[6] = val;
    }
    return buf[6];
}
