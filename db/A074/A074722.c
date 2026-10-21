/*
 * A074722  a(n) = Sum_{d divides n} phi(n/d)*(-1)^bigomega(d).
 * https://oeis.org/A074722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{k=1..n} (-1)^bigomega(gcd(n,k))
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes only positive and negative counts to avoid negative
   intermediate values. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1). */
    if (n == 0) return 0;   /* safety, not used by the tests */

    seq_t pos = 0;          /* count of terms with even Ω(gcd)  */
    seq_t neg = 0;          /* count of terms with odd  Ω(gcd)  */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = gcd((seq_t)n, (seq_t)k);          /* gcd as seq_t   */
        idx_t g_idx = (idx_t)g;                     /* convert back to idx_t for bigomega */
        seq_t omega = bigomega(g_idx);              /* Ω(g) as seq_t  */
        if (omega & 1) {
            /* odd Ω: contributes -1 */
            neg += 1;
        } else {
            /* even Ω: contributes +1 */
            pos += 1;
        }
    }

    return pos - neg;
}
