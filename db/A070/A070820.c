/*
 * A070820  Difference between n-th prime and the value of commutator[phi,gpf]
 *          = commutator[A000010, A006530] at the same prime argument.
 * https://oeis.org/A070820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + gpf(prime(n)-1)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* undefined below offset */
    }
    /* nth prime, 1‑based */
    uint32_t p = get_prime(n);

    /* gpf(p-1), special‑case p = 2 (p-1 = 1) */
    uint64_t m = (uint64_t)p - 1;
    uint64_t gp = (m == 1) ? 1 : gpf(m);

    return (seq_t)1 + (seq_t)gp;
}
