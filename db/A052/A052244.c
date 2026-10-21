/*
 * A052244  Partial sums of A014827.
 * https://oeis.org/A052244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5^(n+3) - (8*n^2 + 44*n + 61)) / 64
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for A052244.
 * Uses direct closed‑form: a(n) = (5^(n+3) - (8*n^2 + 44*n + 61)) / 64.
 * 5^(n+3) is evaluated by exponentiation by squaring. */
static seq_t pow5(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)5;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 5^(n+3) */
    seq_t pow = pow5(n + 3);

    /* polynomial part: 8*n^2 + 44*n + 61 */
    seq_t poly = (seq_t)8 * (seq_t)n * (seq_t)n
               + (seq_t)44 * (seq_t)n
               + (seq_t)61;

    /* numerator */
    seq_t num = pow - poly;

    /* divide by 64 */
    return num / (seq_t)64;
}
