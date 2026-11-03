/*
 * A220161  a(n) = 1 + 2^(2^n) + 2^(2^(n+1)).
 * https://oeis.org/A220161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2^(2^n) + 2^(2^(n+1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t _pow2_of_two_exp(idx_t n)
{
    /* Computes 2^(2^n) by repeated squaring.
       For n = 0, returns 2^(2^0) = 2. */
    seq_t val = 2;
    for (idx_t i = 0; i < n; ++i) {
        val = val * val;   /* val = val^2  (exact integer multiplication) */
    }
    return val;
}

seq_t a(idx_t n)
{
    seq_t b = _pow2_of_two_exp(n);        /* b = 2^(2^n)   */
    seq_t c = b * b;                      /* c = 2^(2^(n+1)) = b^2 */
    return 1 + b + c;                     /* a(n) = 1 + b + c */
}
