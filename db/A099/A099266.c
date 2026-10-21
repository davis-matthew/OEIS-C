/*
 * A099266  Partial sums of A056273.
 * https://oeis.org/A099266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*6^n + 50*4^n + 150*3^n + 675*2^n + 660*n - 878) / 1800
 *            (for n >= 1)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = (seq_t)1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1U) res *= cur;
        cur *= cur;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n) {
    /* compute the powers */
    seq_t p6 = pow_seq((seq_t)6, n);
    seq_t p4 = pow_seq((seq_t)4, n);
    seq_t p3 = pow_seq((seq_t)3, n);
    seq_t p2 = pow_seq((seq_t)2, n);

    /* constants */
    const seq_t c3   = (seq_t)3;
    const seq_t c50  = (seq_t)50;
    const seq_t c150 = (seq_t)150;
    const seq_t c675 = (seq_t)675;
    const seq_t c660 = (seq_t)660;
    const seq_t c878 = (seq_t)878;
    const seq_t den  = (seq_t)1800;

    /* numerator: 3*6^n + 50*4^n + 150*3^n + 675*2^n + 660*n - 878 */
    seq_t num = c3   * p6;
    num += c50  * p4;
    num += c150 * p3;
    num += c675 * p2;
    num += c660 * (seq_t)n;
    num -= c878;

    /* divide by 1800 */
    return num / den;
}
