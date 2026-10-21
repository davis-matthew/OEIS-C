/*
 * A051612  a(n) = sigma(n) - phi(n).
 * https://oeis.org/A051612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - phi(n)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sigma = sigma_of(n);
    seq_t phi_val = phi(n);
    return sigma - phi_val;
}
