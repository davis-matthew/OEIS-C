/*
 * A166033  a(n) = 11/4 +11*n/2 + 29*(-1)^n/4.
 * https://oeis.org/A166033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11 + 22*n + 29*(-1)^n) / 4 (OEIS A166033: a(1)=1,
 *            a(2)=21, a(3)=12, …)
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a signed 64‑bit integer for intermediate arithmetic.
       The values of this sequence for n up to at least 10,000 are well
       within the 64‑bit signed range, so this is safe for every
       seq_t build. */
    long long nn    = (long long)n;
    long long sign  = (n & 1) ? -1LL : 1LL;   /* (-1)^n */
    long long numer = 11LL + 22LL * nn + 29LL * sign;
    return (seq_t)(numer / 4LL);
}
