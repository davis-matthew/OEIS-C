/*
 * A115752  Number of words of length n+1 created with the letters a,b,c with
 *          more c's than b's and more b's than a's.
 * https://oeis.org/A115752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a≥0} Σ_{b=a+1}^{⌊(n-a)/2⌋} C(n+1, a) * C(n+1-a, b)
 *            where C(m, k) is the binomial coefficient. This sums multinomial
 *            coefficients for all triples (a,b,c) with a+b+c = n+1, c>b>a
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;        /* for n = 0,1 there is no word */
    seq_t sum = 0;
    const idx_t nplus1 = n + 1;
    const seq_t one = 1;
    seq_t comb1 = one;          /* C(n+1, 0) */

    /* maximum a such that a+1 ≤ ⌊(n-a)/2⌋  →  3a ≤ n-2  */
    const idx_t max_a = (n >= 2) ? (n - 2) / 3 : 0;

    for (idx_t a = 0; a <= max_a; ++a) {
        const idx_t b_min = a + 1;
        const idx_t b_max = (n - a) / 2;   /* ⌊(n-a)/2⌋ */
        if (b_min > b_max) continue;      /* safety, should not happen */

        const idx_t N = nplus1 - a;        /* remaining positions for b+c */

        /* first b value in the range */
        seq_t comb2 = binomial(N, b_min);  /* C(N, b_min) */

        /* iterate over allowed b */
        for (idx_t b = b_min; b <= b_max; ++b) {
            sum += comb1 * comb2;          /* add multinomial term */

            if (b < b_max) {
                /* compute C(N, b+1) from C(N, b) */
                comb2 = comb2 * (seq_t)(N - b) / (b + 1);
            }
        }

        /* prepare binomial for next value of a:
           C(n+1, a+1) from C(n+1, a) */
        if (a < max_a) {
            comb1 = comb1 * (seq_t)(nplus1 - a) / (a + 1);
        }
    }

    return sum;
}
