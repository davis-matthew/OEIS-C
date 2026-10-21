/*
 * A088606  Smallest number k such that concatenation of k and prime(n) is a
 *          prime, or 0 if no other number exists. a(1) = a(3) = 0.
 * https://oeis.org/A088606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that concatenation of k and prime(n) is
 *            prime, or 0 if no such k exists. The sequence is 1‑based
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   get_prime, is_prime, pow10_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  The auxiliary functions get_prime(idx_t n) and is_prime(idx_t x)
   are available in the OEIS build environment. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/pow10_of.h"

seq_t a(idx_t n)
{
    /* 1‑based indexing: prime(1) = 2   */
    uint32_t p = get_prime(n);
    if (p == 0) return 0;               /* safety guard */

    uint64_t pow10 = pow10_of(p);

    /* Search for the smallest k.  The known b‑file values never require
       k larger than a few million; the inner loop therefore terminates
       long before the hard limit. */
    const uint64_t SEARCH_LIMIT = 2000000ULL;
    for (uint64_t k = 1; k <= SEARCH_LIMIT; ++k) {
        uint64_t candidate = k * pow10 + (uint64_t)p;
        if (is_prime((idx_t)candidate))
            return (seq_t)k;
    }

    /* No suitable k found within the search bound – report 0. */
    return 0;
}
