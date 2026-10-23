/*
 * A049113  Number of powers of 2 in sequence obtained when phi (A000010) is
 *          repeatedly applied to n.
 * https://oeis.org/A049113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of powers of 2 appearing in the iterated Euler
 *            totient sequence starting at n (including the final 1)
 * Range:     u8 n <= 652, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can be computed by repeatedly applying phi and looking
   for powers of two.  The helper functions calc_phi and
   is_power_of_two are provided in the utils library. */
#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    seq_t count = 0;           /* number of powers of two seen */
    idx_t x = n;               /* current value of the iterated sequence */

    /* Count the starting term if it is a power of two. */
    if (is_power_of_two((seq_t)x))
        count++;

    /* Iterate phi until we reach 1, counting powers of two. */
    while (x > 1) {
        seq_t phi_val = calc_phi((seq_t)x);   /* phi(x) */
        x = (idx_t)phi_val;                   /* safe cast: phi(x) ≤ x */
        if (is_power_of_two(phi_val))
            count++;
    }

    return count;
}
