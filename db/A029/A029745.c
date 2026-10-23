/*
 * A029745  Expansion of (1 + 2x + 6x^2 + x^3)/(1 - 2x^2).
 * https://oeis.org/A029745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n=1; a(2)=2, a(4)=5; for odd n≥3: a(n)=2^((n+3)/2);
 *            for even n≥6: a(n)=5 * 2^((n/2)-2)
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 4) return (seq_t)5;

    /* Odd indices (n≥3) */
    if (n & (idx_t)1) {
        idx_t exp = (n + 3) / 2;          /* exponent of 2 */
        return ((seq_t)1 << exp);        /* 2^exp */
    }

    /* Even indices (n≥6) */
    idx_t half = n / 2;                  /* n = 2*half */
    idx_t exp = half - 2;                /* exponent for power of 2 term 5*2^exp */
    return ((seq_t)5 << exp);            /* 5 * 2^exp */
}
