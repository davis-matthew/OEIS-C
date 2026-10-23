/*
 * A144843  a(n) = (6^n - 2^n)^2 / 16.
 * https://oeis.org/A144843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6^n - 2^n)^2 / 16
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n ≥ 2 the equivalent compact form is: */
/* a(n) = 81*36^(n-2) + 4^(n-2) - 18*12^(n-2). */
/* The implementation uses only integer arithmetic, no floating point, */
/* and works correctly for all n for which a(n) fits in the type seq_t. */

static seq_t big_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > (idx_t)0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;                 /* a(1) = 1 */

    /* n ≥ 2 */
    idx_t exp = n - (idx_t)2;

    /* Compute powers needed in the compact formula. */
    seq_t pow36 = big_pow((seq_t)36, exp);   /* 36^(n-2)  */
    seq_t pow4  = big_pow((seq_t)4,  exp);   /* 4^(n-2)   */
    seq_t pow12 = big_pow((seq_t)12, exp);   /* 12^(n-2)  */

    /* a(n) = 81*36^(n-2) + 4^(n-2) - 18*12^(n-2). */
    seq_t res = (seq_t)81 * pow36;
    res += pow4;
    res -= (seq_t)18 * pow12;
    return res;
}
