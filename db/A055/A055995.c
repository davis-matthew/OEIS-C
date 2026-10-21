/*
 * A055995  a(n) = 64*9^(n-2), a(0) = 1, a(1) = 7.
 * https://oeis.org/A055995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 64 * 9^(n-2) for n >= 2; a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    idx_t exp = n - 2;          /* exponent to which 9 is raised */
    seq_t base = (seq_t)9;      /* base of exponentiation   */
    seq_t res = (seq_t)1;       /* result of exponentiation */

    /* fast binary exponentiation */
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }

    return (seq_t)64 * res;     /* final term: 64 * 9^(n-2) */
}
