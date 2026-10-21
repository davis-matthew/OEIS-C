/*
 * A055270  a(n) = 7*a(n-1) + (-1)^n * binomial(2,2-n) with a(-1)=0.
 * https://oeis.org/A055270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 36 * 7^(n-2) for n ≥ 2, a(0) = 1, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence  a(n) = 7*a(n-1) + (-1)^n * binomial(2,2-n) with a(-1)=0
   collapses to this closed form. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;

    /* n ≥ 2: compute 7^(n-2) by binary exponentiation */
    idx_t exp = n - 2;
    seq_t base = (seq_t)7;
    seq_t result = (seq_t)1;

    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }

    return result * (seq_t)36;
}
