/*
 * A170763  Expansion of g.f.: (1+x)/(1-43*x).
 * https://oeis.org/A170763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=44; a(n)=43*a(n-1) for n>=2 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 44;

    /* compute 44*43^(n-1) via binary exponentiation */
    seq_t base = 43;
    seq_t res  = 1;
    idx_t exp  = n - 1;

    while (exp > 0) {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res * 44;
}
