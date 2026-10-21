/*
 * A081681  a(n) = (8^n - 7^n - 6^n - 5^n + 4*4^n)/2.
 * https://oeis.org/A081681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8^n - 7^n - 6^n - 5^n + 4*4^n)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base^exp for unsigned seq_t using exponentiation by squaring.
   The helper is static to avoid name clashes. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p8 = pow_seq_t((seq_t)8, n);
    seq_t p7 = pow_seq_t((seq_t)7, n);
    seq_t p6 = pow_seq_t((seq_t)6, n);
    seq_t p5 = pow_seq_t((seq_t)5, n);
    seq_t p4 = pow_seq_t((seq_t)4, n);

    /* Compute (8^n + 4*4^n - 7^n - 6^n - 5^n) / 2.
       The addition of 8^n and 4*4^n is safe as it is always
       ≥ the sum of the subtracted terms for all n ≥ 0. */
    seq_t sum = p8 + 4 * p4;
    seq_t diff = sum - p7 - p6 - p5;
    return diff / 2;
}
