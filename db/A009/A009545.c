/*
 * A009545  Expansion of e.g.f. sin(x)*exp(x).
 * https://oeis.org/A009545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = -4*a(n-4) for n>3 with a(0)=0, a(1)=1, a(2)=2, a(3)=2
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 64, i64 n <= 128,
 *            i128 n <= 256, big any n
 * Validated: standard, pass, 67 terms through n = 1955 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes (−4)^exp for non‑negative exp */
static seq_t neg_four_pow(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = (seq_t)-4;
    while (exp)
    {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Base values for n = 0,1,2,3 */
    static const seq_t base[4] = {0, 1, 2, 2};

    if (n < 4) return base[n];

    idx_t k   = n / 4;          /* number of 4‑step cycles   */
    seq_t pw  = neg_four_pow(k);/* (‑4)^k                    */
    return base[n % 4] * pw;    /* combine with base value   */
}
