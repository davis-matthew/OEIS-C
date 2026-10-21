/*
 * A053111  Expansion of (-1 + 1/(1-8*x)^8)/(64*x); related to A053107.
 * https://oeis.org/A053111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A053111:  a(n) = 8^(n‑1) * binomial(n+8, 7)  (a(0) = 1) */
seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    /* Compute binomial coefficient C(n+8, 7) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 7; ++i) {
        seq_t numer = (seq_t)(n + 1 + i);   /* (n+1)+i = (n+2)…(n+8)   */
        binom = binom * numer / (seq_t)i;   /* division is exact          */
    }

    /* Compute 8^(n‑1) using exponentiation by squaring */
    seq_t pow8 = 1;
    seq_t base = (seq_t)8;
    idx_t exp = n - 1;
    while (exp > 0) {
        if (exp & 1LL)
            pow8 = pow8 * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }

    return pow8 * binom;
}
