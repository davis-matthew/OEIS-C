/*
 * A198847  a(n) = 10*6^n - 1.
 * https://oeis.org/A198847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*6^n - 1
 * Recurrence: a(0) = 9, a(n) = 6*a(n-1) + 5
 *
 * Computes 6^n using binary exponentiation and then returns 10*6^n - 1.
 * Works for seq_t types that support standard arithmetic operations.
 */
seq_t a(idx_t n)
{
    seq_t pow6 = (seq_t)1;   /* 6^0 */
    seq_t base  = (seq_t)6;  /* current base (6, 36, 1296, ...) */
    idx_t e = n;

    while (e)
    {
        if (e & 1ULL)
            pow6 = pow6 * base;
        base = base * base;
        e >>= 1ULL;
    }

    return pow6 * (seq_t)10 - (seq_t)1;
}
