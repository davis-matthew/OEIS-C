/*
 * A138256  Smallest positive integer m such that n divides [3^m/m]
 *          (=A092763(m)).
 * https://oeis.org/A138256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A138256:
    a(n) = smallest positive integer m such that
    n divides floor(3^m / m) = A092763(m)
*/

#include <stdint.h>
#include "../utils/mod_pow.h"

/* -------------------------------------------------------------
   Compute a(n) by brute‑force search on m.
   For each m we check whether 3^m mod (n*m) equals 3^m mod m.
   This is equivalent to n | floor(3^m / m).
   ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for 0, return 0 */

    for (idx_t m = 1; ; ++m) {
        /* r = 3^m (mod m) */
        seq_t r = mod_pow((seq_t)3, (uint64_t)m, (seq_t)m);

        /* nm = n * m */
        seq_t nm = (seq_t)n * (seq_t)m;

        /* t = 3^m (mod n*m) */
        seq_t t = mod_pow((seq_t)3, (uint64_t)m, nm);

        if (t == r)           /* n | floor(3^m/m) */
            return (seq_t)m;
    }
}
