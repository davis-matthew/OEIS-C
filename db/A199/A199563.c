/*
 * A199563  a(n) = 5*9^n + 1.
 * https://oeis.org/A199563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*9^n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 9^n is computed by fast exponentiation */
seq_t a(idx_t n)
{
    seq_t base = 9;   /* base = 9 */
    seq_t res  = 1;   /* result = 1 */

    while (n) {
        if (n & 1)    /* if current bit is set, multiply result by base */
            res = res * base;
        base = base * base;  /* square the base for next bit */
        n >>= 1;
    }

    res = res * 5 + 1;  /* final a(n) = 5*9^n + 1 */
    return res;
}
