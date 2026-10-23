/*
 * A013887  a(n) = 18^(5*n + 2).
 * https://oeis.org/A013887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18^(5*n + 2) (OEIS A013887)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 324, a(n) = 1889568 * a(n-1)  (since 18^5 = 1889568) */

seq_t a(idx_t n)
{
    seq_t result   = (seq_t)1;           /* will hold 1889568^n  */
    seq_t power    = (seq_t)1889568;     /* base = 18^5 */
    idx_t   exp    = n;

    /* exponentiation by squaring: result = power^exp */
    while (exp != 0) {
        if (exp & 1ULL) {
            result *= power;
        }
        power *= power;
        exp >>= 1;
    }

    /* a(n) = 324 * (1889568^n) */
    return (seq_t)324 * result;
}
