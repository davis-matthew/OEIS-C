/*
 * A010125  Continued fraction for sqrt(21).
 * https://oeis.org/A010125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = gcd(Fibonacci(n), Fibonacci(n-6)), n > 0
 * Range:     u8 n <= 9817, u16 n <= 9817, u32 n <= 9817, u64 n <= 9817,
 *            u128 n <= 9817, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence value a(0) = 4  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;          /* first term */

    /* gcd(n,6) depends only on n mod 6:
       n mod 6 == 0 -> gcd = 6  -> F_6 = 8
       n mod 6 == 1 or 5 -> gcd = 1 -> F_1 = 1
       n mod 6 == 2 or 4 -> gcd = 2 -> F_2 = 1
       n mod 6 == 3      -> gcd = 3 -> F_3 = 2   */
    switch (n % 6)
    {
        case 0: return (seq_t)8;  /* gcd = 6, F_6 = 8 */
        case 1: return (seq_t)1;  /* gcd = 1, F_1 = 1 */
        case 2: return (seq_t)1;  /* gcd = 2, F_2 = 1 */
        case 3: return (seq_t)2;  /* gcd = 3, F_3 = 2 */
        case 4: return (seq_t)1;  /* gcd = 2, F_2 = 1 */
        case 5: return (seq_t)1;  /* gcd = 1, F_1 = 1 */
        default: return (seq_t)0; /* unreachable */
    }
}
