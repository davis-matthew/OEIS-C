/*
 * A094629  Erroneous version of A052216.
 * https://oeis.org/A094629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = { 2 * 10^((n-1)/2) , if n is odd 11 * 10^((n/2)-1) , if n
 *            is even } (offset 1) Uses constant‑time multiplication by 10 to
 *            build the power of ten
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow10_local(idx_t e) {
    seq_t r = (seq_t)1;
    while (e--) {
        r *= (seq_t)10;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                /* offset starts at 1 */

    if ((n & (idx_t)1) == (idx_t)1) {           /* odd n */
        idx_t k = (n - 1) / 2;
        seq_t p = pow10_local(k);
        return (seq_t)2 * p;
    } else {                                    /* even n */
        idx_t k = n / 2 - 1;
        seq_t p = pow10_local(k);
        return (seq_t)11 * p;
    }
}
