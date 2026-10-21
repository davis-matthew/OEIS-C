/*
 * A219093  Denominator of the least reduced fraction b/c > 1 using divisors b
 *          and c of n.
 * https://oeis.org/A219093
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = denominator of the least reduced fraction b/c > 1 where b
 *            and c are divisors of n and b*c divides n. Brute‑force O(τ(n)²)
 *            search, τ(n) = number of divisors
 * Range:     u8 n <= 6774, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n<2 */

    /* ---------------------------------------------------
       Collect all positive divisors of n.
       --------------------------------------------------- */
    enum { MAX_DIV = 10000 };      /* enough for n up to ~10^12   */
    idx_t divs[MAX_DIV];
    size_t dcnt = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (dcnt < MAX_DIV) divs[dcnt++] = i;
            idx_t j = n / i;
            if (j != i && dcnt < MAX_DIV) divs[dcnt++] = j;
        }
    }

    /* ---------------------------------------------------
       Search all pairs (b,c) with b>c, bc | n, and
       choose the one yielding the minimal fraction b/c.
       --------------------------------------------------- */
    idx_t best_b = 0;
    idx_t best_c = 0;

    for (size_t ii = 0; ii < dcnt; ++ii) {
        idx_t c = divs[ii];
        for (size_t jj = 0; jj < dcnt; ++jj) {
            idx_t b = divs[jj];
            if (b <= c) continue;                     /* need b/c > 1 */

            if (b > n / c) continue;                 /* would overflow & > n */
            idx_t prod = b * c;                       /* now safe, prod <= n */

            if (n % prod != 0) continue;              /* bc must divide n */

            /* Compare b/c with the best fraction found so far. */
            if (best_b == 0 ||
                ((seq_t)b) * best_c < ((seq_t)best_b) * c)
            {
                best_b = b;
                best_c = c;
            }
        }
    }

    /* ---------------------------------------------------
       Reduce the fraction best_b / best_c and return
       the denominator.
       --------------------------------------------------- */
    seq_t g = gcd((seq_t)best_b, (seq_t)best_c);
    seq_t denom = ((seq_t)best_c) / g;
    return denom;
}
