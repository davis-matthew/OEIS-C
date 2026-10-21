/*
 * A076738  Expansion of (1+2*x+6*x^2)/(1-9*x^3).
 * https://oeis.org/A076738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^⌊n/3⌋ * c(n), where c(n) = 1, 2, 6 for n ≡ 0, 1, 2 (mod
 *            3) respectively
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 9^floor(n/3) with exponentiation by squaring */
    idx_t k = n / 3;
    seq_t power = 1;
    seq_t base  = 9;
    while (k > 0) {
        if (k & 1) power *= base;
        base *= base;
        k >>= 1;
    }

    /* multiply by the constant determined by n mod 3 */
    switch (n % 3) {
        case 0: return power * (seq_t)1;
        case 1: return power * (seq_t)2;
        case 2: return power * (seq_t)6;
    }
    return 0;  /* unreachable */
}
