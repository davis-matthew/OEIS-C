/*
 * A193532  G.f.: x = Sum_{n>=1} x^n * ((1+x)^(n+1) - x^(n+1)) / (1+x)^a(n).
 * https://oeis.org/A193532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(log2(n+1)) + sum_{k>=1} floor((n+1)/2^k)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute exponent of 2 in (k)!  (Legendre formula) */
static seq_t _exp2_factorial(idx_t k)
{
    seq_t e = 0;
    idx_t m = k >> 1;           /* start with floor(k/2) */
    while (m) {
        e += (seq_t)m;          /* add floor(k/2^i) */
        m >>= 1;                /* divide by 2 */
    }
    return e;
}

seq_t a(idx_t n)
{
    /* n is 1-based: n >= 1 */
    idx_t np1 = n + 1;          /* compute with idx_t to avoid overflow of n+1 */
    seq_t e = _exp2_factorial(np1);  /* exponent of 2 in (n+1)! */

    /* floor(log2(np1)) */
    seq_t log2 = 0;
    idx_t t = np1;
    while (t >> 1) {            /* while t >= 2 */
        t >>= 1;
        ++log2;
    }

    return (seq_t)n + log2 + e; /* final value */
}
