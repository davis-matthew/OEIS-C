/*
 * A211219  Maximum value of sigma(x) * sigma(y) * sigma(z), where x + y + z =
 *          n.
 * https://oeis.org/A211219
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{x+y+z=n, x,y,z≥1} sigma(x)*sigma(y)*sigma(z)
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   maximum_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma is the sum–of–divisors function */

#include <stdlib.h>   /* calloc, free */
#include <stdint.h>
#include <stddef.h>
#include "../utils/maximum_product.h"

/* external entry point required by the build system */
seq_t a(idx_t n)
{
    if (n < 3) return 0;               /* undefined below offset 3 */
    return maximum_product(n);
}
