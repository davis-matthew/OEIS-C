/*
 * A064758  a(n) = n*12^n - 1.
 * https://oeis.org/A064758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*12^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 146 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 12^exp using binary exponentiation on seq_t values. */
static seq_t exp12(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = (seq_t)12;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 as per the OEIS offset. */
    seq_t pow_val = exp12(n);          /* 12^n        */
    seq_t res     = (seq_t)n * pow_val;/* n * 12^n    */
    res -= 1;                          /* minus 1     */
    return res;
}
