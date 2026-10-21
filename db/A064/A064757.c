/*
 * A064757  a(n) = n*11^n - 1.
 * https://oeis.org/A064757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*11^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A064757 for any n >= 1. */
/* Uses exponentiation by squaring for O(log n) multiplications. */

seq_t a(idx_t n)
{
    /* Compute base = 11^n in which seq_t is an arbitrary‑precision or
       unsigned integer type. */
    seq_t base = (seq_t)11;
    seq_t power = 1;            /* 11^n will be stored here. */
    idx_t exp = n;              /* Use a copy of n for bit‑shifting. */

    while (exp > 0) {
        if (exp & 1) {
            power *= base;
        }
        base *= base;
        exp >>= 1;
    }

    /* Multiply by n and subtract 1. */
    power *= (seq_t)n;          /* now power = n * 11^n  */
    power -= 1;                 /* result = n*11^n - 1 */

    return power;
}
