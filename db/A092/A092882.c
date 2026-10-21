/*
 * A092882  Number of ordered 4-multiantichains on an n-set.
 * https://oeis.org/A092882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n - 12*12^n + 24*10^n + 4*9^n - 12*8^n + 6*7^n -
 *            72*6^n + 72*5^n + 36*4^n - 72*3^n + 26*2^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation: computes base^exp in type seq_t */
static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n) {
    /* compute positive contributions */
    seq_t pos =
        seq_pow((seq_t)16, n)
        + (seq_t)24 * seq_pow((seq_t)10, n)
        + (seq_t)4  * seq_pow((seq_t)9 , n)
        + (seq_t)6  * seq_pow((seq_t)7 , n)
        + (seq_t)72 * seq_pow((seq_t)5 , n)
        + (seq_t)36 * seq_pow((seq_t)4 , n)
        + (seq_t)26 * seq_pow((seq_t)2 , n);

    /* compute negative contributions */
    seq_t neg =
        (seq_t)12 * seq_pow((seq_t)12, n)
        + (seq_t)12 * seq_pow((seq_t)8 , n)
        + (seq_t)72 * seq_pow((seq_t)6 , n)
        + (seq_t)72 * seq_pow((seq_t)3 , n);

    /* pos >= neg for all n >= 0, so subtraction cannot underflow */
    return pos - neg;
}
