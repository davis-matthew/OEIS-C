/*
 * A052656  E.g.f. x*(1+2x-4x^2)/(1-2x).
 * https://oeis.org/A052656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0,1,8,2^(n-1)*n! for n>=3 (recurrence provided for
 *            reference)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=8, a(n)=2^(n-1)*n! for n>=3 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 8;

    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;

    /* compute pow2 = 2^(n-1) via exponentiation by squaring */
    seq_t pow2 = 1;
    seq_t base = (seq_t)2;
    idx_t e = n - 1;
    while (e) {
        if (e & 1) pow2 *= base;
        base *= base;
        e >>= 1;
    }

    result *= pow2;
    return result;
}
