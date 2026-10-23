/*
 * A034135  Decimal part of cube root of n starts with 9: first term of runs.
 * https://oeis.org/A034135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil(((10n+9)^3)/1000) (OEIS A034135)
 * Range:     u8 none, u16 n <= 3, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;          /* off‑by‑one safety */
    seq_t tn  = (seq_t)n;                   /* cast index to seq_t */
    seq_t t   = (seq_t)10 * tn + (seq_t)9;  /* 10n + 9   */
    seq_t cube = t * t * t;                 /* (10n+9)^3 */
    return (cube + (seq_t)999) / (seq_t)1000;
}
