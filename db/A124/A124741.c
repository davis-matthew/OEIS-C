/*
 * A124741  a(n) = largest of those positive integers which are coprime to
 *          both n and n+1 and which are <= n.
 * https://oeis.org/A124741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest integer k ≤ n such that gcd(k,n)=1 and
 *            gcd(k,n+1)=1
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 */
    if (n <= 1) {
        return 1;
    }

    /* Even n → answer is n-1 (since n-1 is odd and coprime to both n and n+1) */
    if ((n & 1) == 0) {
        return (seq_t)n - 1;
    }

    /* n is odd and ≥ 3 */
    seq_t n_seq    = (seq_t)n;
    seq_t np1_seq  = n_seq + 1;
    seq_t k        = n_seq - 2;   /* first odd candidate below n */

    for ( ; k >= 1; ) {
        /* Check coprimality with both n and n+1 */
        if (gcd(k, n_seq) == 1 && gcd(k, np1_seq) == 1) {
            return k;
        }

        /* Prevent infinite loop due to unsigned wrap‑around */
        if (k <= 2) {
            break;
        }
        k -= 2;   /* next smaller odd candidate */
    }

    /* Fallback: 1 is always coprime to everything */
    return 1;
}
