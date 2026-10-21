/*
 * A055811  a(n) = T(n,n-6), array T as in A055807.
 * https://oeis.org/A055811
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1584 - 310*n - 85*n^2 + 10*n^3 + n^4)/120 for n > 6,
 *            a(6)=1
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 218, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 6) {
        return (n == 6) ? (seq_t)1 : (seq_t)0;
    }
    seq_t s  = n;
    seq_t s2 = s * s;
    seq_t s3 = s2 * s;
    seq_t s4 = s3 * s;

    seq_t sum = (seq_t)1584 + ((seq_t)10 * s3) + s4;
    sum -= ((seq_t)310 * s);
    sum -= ((seq_t)85 * s2);

    seq_t a_val = s;
    seq_t b_val = sum;

    /* divide by 5 */
    if (a_val % 5 == 0) {
        a_val /= 5;
    } else {
        b_val /= 5;
    }
    /* divide by 3 */
    if (a_val % 3 == 0) {
        a_val /= 3;
    } else {
        b_val /= 3;
    }
    /* divide by 2 three times */
    for (int i = 0; i < 3; ++i) {
        if (a_val % 2 == 0) {
            a_val /= 2;
        } else {
            b_val /= 2;
        }
    }

    return a_val * b_val;
}
