/*
 * A024154  Number of integer-sided triangles with sides a,b,c, a<b<c, a+b+c=n
 *          that are acute.
 * https://oeis.org/A024154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count integer triples (a,b,c) with a<b<c, a+b+c=n that form an
 *            acute triangle (i.e. a^2+b^2>c^2)
 * Range:     u8 n <= 32, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Minimal possible sum for a<b<c is 1+2+3 = 6 */
    if (n < 6) return count;

    /* a max: a + (a+1) + (a+2) <= n  → 3a+3 <= n  → a <= (n-3)/3 */
    for (idx_t a = 1; a <= (n - 3) / 3; ++a) {
        /* b minimum to satisfy a < b */
        idx_t b_min = a + 1;
        /* b minimum to satisfy triangle inequality a+b > n/2
           which is equivalent to b > (n-2a)/2 */
        idx_t H = (n > 2*a ? (n - 2*a) / 2 : 0);
        if (H + 1 > b_min) b_min = H + 1;

        /* b maximum to satisfy c > b i.e. n - a - b > b → b < (n-a)/2
           b integer < (n-a)/2 → b <= floor((n-a-1)/2) */
        idx_t b_max = (n - a - 1) / 2;
        if (b_min > b_max) continue;

        for (idx_t b = b_min; b <= b_max; ++b) {
            idx_t c = n - a - b;   /* automatically c > b by construction */

            /* Check acute: a^2 + b^2 > c^2 */
            seq_t asq = (seq_t)a * a;
            seq_t bsq = (seq_t)b * b;
            seq_t csq = (seq_t)c * c;
            if (asq + bsq > csq) {
                count = count + 1;
            }
        }
    }
    return count;
}
