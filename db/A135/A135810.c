/*
 * A135810  Number of coincidence-free length n lists of 4-tuples with all
 *          numbers 1..n in tuple position k, for k=1..4.
 * https://oeis.org/A135810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..n} ((-1)^(n-j))*binomial(n,j)*(j!)^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;
    seq_t binom = 1;   /* C(n,0) */
    seq_t fact4 = 1;   /* (0!)^4 */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = binom * fact4;
        if (((n - j) & 1) != 0) {
            neg += term;
        } else {
            pos += term;
        }

        if (j == n) break;  /* no next iteration */

        /* update factorial^4: new_fact4 = old_fact4 * (j+1)^4 */
        seq_t mult = (seq_t)(j + 1);
        seq_t mult2 = mult * mult;     /* (j+1)^2 */
        seq_t mult4 = mult2 * mult2;   /* (j+1)^4 */
        fact4 *= mult4;

        /* update binomial coefficient: C(n,j+1) = C(n,j) * (n-j) / (j+1) */
        binom *= (seq_t)(n - j);
        binom /= (seq_t)(j + 1);
    }
    return pos - neg;
}
