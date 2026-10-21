/*
 * A097629  a(n) = 2*(2*n)^(n-2).
 * https://oeis.org/A097629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (2*n)^(n-2) for n ≥ 2, with a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) {
            res *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;          /* a(1) = 1 */
    }

    seq_t base = ((seq_t)2 * (seq_t)n);  /* base = 2*n */
    idx_t exp  = n - 2;                 /* exponent = n-2 */

    seq_t power = pow_seq_t(base, exp);  /* compute (2*n)^(n-2) */

    return (seq_t)2 * power;           /* a(n) = 2 * (2*n)^(n-2) */
}
