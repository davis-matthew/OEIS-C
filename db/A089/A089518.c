/*
 * A089518  Third column (k=5) of array A078741 ((3,3)-Stirling2) divided by
 *          9.
 * https://oeis.org/A089518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*60^n - 8*24^n + 6^n) / 3 (OEIS A089518)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any seq_t / idx_t that supports the basic arithmetic operators. */

#ifndef A089518_H
#define A089518_H

/* Fast exponentiation for seq_t, using idx_t as exponent type */
static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* constants as seq_t */
    const seq_t C10 = (seq_t)10;
    const seq_t C8  = (seq_t)8;
    const seq_t C60 = (seq_t)60;
    const seq_t C24 = (seq_t)24;
    const seq_t C6  = (seq_t)6;
    const seq_t C3  = (seq_t)3;

    seq_t p60 = seq_pow(C60, n);   /* 60^n   */
    seq_t p24 = seq_pow(C24, n);   /* 24^n   */
    seq_t p6  = seq_pow(C6 , n);   /* 6^n    */

    /* compute (10*60^n - 8*24^n + 6^n)/3 */
    seq_t num = C10 * p60 - C8 * p24 + p6;
    return num / C3;
}

#endif /* A089518_H */
