/*
 * A052777  E.g.f.: x^2*(exp(x)-1)^3.
 * https://oeis.org/A052777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*(3^(n-2) - 3*2^(n-2) + 3) for n ≥ 5,
 *            a(0)=a(1)=a(2)=a(3)=a(4)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute base^exp with exponentiation by squaring, using seq_t arithmetic.
 * This works for the arbitrary‑precision build (where seq_t overloads *)
 * and for the fixed‑size integer builds (wrapping on overflow, which
 * only affects terms that already exceed seq_t). */
static seq_t pow_big(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U)
            result = result * base;
        exp >>= 1U;
        if (exp)
            base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Offset 0: first 5 terms are zero. */
    if (n < 5)
        return 0;

    /* powers needed in the formula */
    seq_t pow3 = pow_big((seq_t)3, n - 2);
    seq_t pow2 = pow_big((seq_t)2, n - 2);

    /* inner bracket: 3^(n-2) - 3*2^(n-2) + 3 */
    seq_t inside = pow3 - (seq_t)3 * pow2 + (seq_t)3;

    /* final product: n*(n-1)*inside */
    seq_t temp   = (seq_t)n * (seq_t)(n - 1);
    temp *= inside;
    return temp;
}
