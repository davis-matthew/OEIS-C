/*
 * A140402  Expansion of x^4/((1-x)^2*(1-x^3)) + x^5/((1-x)*(1-x^2)*(1-x^5)).
 * https://oeis.org/A140402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{b=0..⌊(n-4)/3⌋} (n-3b-3) + Σ_{c=0..⌊(n-5)/5⌋}
 *            (⌊(n-5-5c)/2⌋+1)
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (derived from the expansion of x^4/((1-x)^2*(1-x^3)) + x^5/((1-x)*(1-x^2)*(1-x^5)) ) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;                /* first non‑zero term is at n = 4 */

    seq_t result = 0;

    /* ----- part from x^4/((1-x)^2*(1-x^3)) ----- */
    idx_t m = (n - 4) / 3;              /* number of terms minus 1   */
    seq_t k = (seq_t)(m + 1);           /* number of terms */
    seq_t part1 = k * ((seq_t)n - 3) - (seq_t)3 * k * (seq_t)m / 2;
    result += part1;

    /* ----- part from x^5/((1-x)*(1-x^2)*(1-x^5)) ----- */
    if (n >= 5) {
        idx_t t = n - 5;
        idx_t L = t / 5;                /* maximum value of c   */
        seq_t part2 = 0;
        for (idx_t c = 0; c <= L; ++c) {
            idx_t S = t - 5 * c;        /* remaining sum for a+2b */
            part2 += (S / 2) + 1;       /* floor((S)/2)+1  */
        }
        result += part2;
    }

    return result;
}
