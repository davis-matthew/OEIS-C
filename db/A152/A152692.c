/*
 * A152692  a(n) = n*3^n - n*2^n - n*1^n.
 * https://oeis.org/A152692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3^n - 2^n - 1) (OEIS A152692)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;   /* start at 1 */
    while (exp) {
        result *= base;
        --exp;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* a(0) = 0  */

    seq_t pow3 = pow_seq((seq_t)3, n);  /* 3^n  */
    seq_t pow2 = pow_seq((seq_t)2, n);  /* 2^n  */

    seq_t diff = pow3 - pow2;           /* 3^n - 2^n */
    diff -= 1;                          /* 3^n - 2^n - 1 */

    return diff * (seq_t)n;             /* n*(3^n - 2^n - 1) */
}
