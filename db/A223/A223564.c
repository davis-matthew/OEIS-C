/*
 * A223564  Number of nX3 0..1 arrays with antidiagonals unimodal.
 * https://oeis.org/A223564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8, a(2) = 64, a(n) = 7*a(n-1) for n > 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 1)
        return (seq_t)8;
    if (n == 2)
        return (seq_t)64;

    /* For n >= 3 use the recurrence a(n) = 7 * a(n-1).
       Compute 7^(n-2) via exponentiation by squaring. */
    idx_t e = n - 2;          /* exponent */
    seq_t result = (seq_t)1;  /* 7^0 */
    seq_t base   = (seq_t)7;  /* 7^1 */

    while (e) {
        if (e & 1U)
            result = result * base;
        base = base * base;
        e >>= 1U;
    }

    /* a(n) = 64 * 7^(n-2) for n >= 2 */
    return (seq_t)64 * result;
}
