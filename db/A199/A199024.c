/*
 * A199024  a(n) = 6*11^n - 1.
 * https://oeis.org/A199024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*11^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* local helper: exponentiation by squaring for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* compute a(n) for the sequence A199024 */
seq_t a(idx_t n)
{
    seq_t p = pow_seq((seq_t)11, n);          /* 11^n          */
    seq_t r = p * (seq_t)6;                  /* 6*11^n        */
    return r - (seq_t)1;                     /* 6*11^n - 1    */
}
