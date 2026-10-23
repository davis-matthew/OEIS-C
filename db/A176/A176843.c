/*
 * A176843  The number of iterations of the map n -> n - bigomega(phi(n))
 *          until reaching 2.
 * https://oeis.org/A176843
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of iterations of f(x)=x-bigomega(phi(x)) until x=2
 *            a(2)=0, a(3)=1, a(4)=2, a(5)=2, a(6)=3, …
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   bigomega, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour below offset is irrelevant   */
    idx_t current = n;            /* current value in the iteration        */
    seq_t steps   = 0;            /* number of steps taken                 */

    while (current != 2) {
        /* reduce current by the bigomega of its totient value   */
        idx_t phi_val = (idx_t)phi(current);
        idx_t sub     = (idx_t)bigomega(phi_val);
        current -= sub;
        steps++;
    }
    return steps;
}
