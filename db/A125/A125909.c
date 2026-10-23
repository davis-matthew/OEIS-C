/*
 * A125909  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks the digit 1, at least
 *          one of the digits 2,3 and at least one of digits 4,5,6,7,8,9.
 * https://oeis.org/A125909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 12*7^n-36*6^n+55*5^n-50*4^n+27*3^n-8*2^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute base^exp using exponentiation by squaring. */
static inline seq_t seq_pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p2 = seq_pow_seq((seq_t)2, n);
    seq_t p3 = seq_pow_seq((seq_t)3, n);
    seq_t p4 = seq_pow_seq((seq_t)4, n);
    seq_t p5 = seq_pow_seq((seq_t)5, n);
    seq_t p6 = seq_pow_seq((seq_t)6, n);
    seq_t p7 = seq_pow_seq((seq_t)7, n);

    seq_t sum_pos = ((seq_t)12) * p7
                  + ((seq_t)55) * p5
                  + ((seq_t)27) * p3
                  + (seq_t)1;

    seq_t sum_neg = ((seq_t)36) * p6
                  + ((seq_t)50) * p4
                  + ((seq_t)8)  * p2;

    return sum_pos - sum_neg;
}
