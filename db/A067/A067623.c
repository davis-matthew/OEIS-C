/*
 * A067623  Consider the power series (x+1)^(1/3)=1+x/3-x^2/9+5x^3/81+...;
 *          sequence gives denominators of coefficients.
 * https://oeis.org/A067623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 3^( n + ⌊n/3⌋ + ⌊n/9⌋ + ⌊n/27⌋ + … ) Recurrence: a(n) =
 *            3^n * a(⌊n/3⌋), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 3;
    while (exp > 0) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t exp = 0;
    idx_t t   = n;
    while (t > 0) {
        exp += t;
        t   /= 3;
    }
    return pow3(exp);
}
