/*
 * A062749  Sixth column (r=5) of FS(3) staircase array A062745.
 * https://oeis.org/A062749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -3 + binomial(n+4,3)*(n^2+16*n+75)/20
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 215, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 0 … 5 (not strictly needed but kept for safety) */
    static const seq_t base[] = {12, 43, 108, 228, 431, 753};

    if (n < 6) return base[n];

    /* Work in unsigned __int128 to avoid overflow for 64‑bit builds.
       The final value always fits into seq_t for n ≤ 10000, which is
       the required range.  For larger n the intermediate value still
       fits in 128 bits because the sequence grows as n^5/120. */
    unsigned __int128 n128 = (unsigned __int128)n;

    unsigned __int128 factor = (n128 + 4) * (n128 + 3) * (n128 + 2);   /* (n+4)(n+3)(n+2) */
    unsigned __int128 poly    = n128 * n128 + 16 * n128 + 75;           /* n^2 + 16n + 75 */

    unsigned __int128 numerator = factor * poly;                       /* product of the two */
    unsigned __int128 div120    = numerator / 120;                     /* divide by 6*20 = 120 */

    /* Convert back to seq_t.  For fixed‑width types this cast truncates
       to the correct size; for arbitrary‑precision types it constructs
       the value from the small integer. */
    seq_t result = (seq_t)div120 - (seq_t)3;
    return result;
}
