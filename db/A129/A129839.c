/*
 * A129839  a(n) = Stirling_2(n,3)^2.
 * https://oeis.org/A129839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - 3·2^n + 3)^2 / 36
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   for n >= 0, with a(0) = a(1) = a(2) = 0            */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* A129839 is defined only for n >= 0;
       the first three terms are zero.            */
    if (n <= 2) return 0;

    /* Compute 3^n and 2^n.  For the 64‑bit build
       these values are small enough to avoid
       overflow when stored directly in seq_t.   */
    seq_t pow3 = pow_seq(3, n);
    seq_t pow2 = pow_seq(2, n);

    /* a(n) = ((3^n - 3·2^n + 3) / 6)^2  */
    seq_t temp = pow3 - (pow2 * 3) + 3;   /* numerator of the division   */
    seq_t half = temp / 6;                /* guaranteed integer          */
    return half * half;
}
