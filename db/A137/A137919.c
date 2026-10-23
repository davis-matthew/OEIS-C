/*
 * A137919  (Nextprime(24n)-previousprime(24n))/2.
 * https://oeis.org/A137919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Nextprime(24n)-previousprime(24n))/2
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    idx_t base = 24 * n;

    /* find previous prime <= base */
    idx_t prev = base;
    while (!is_prime(prev)) {
        --prev;
    }

    /* find next prime >= base */
    idx_t next = base;
    while (!is_prime(next)) {
        ++next;
    }

    /* compute (next - prev) / 2 */
    return (seq_t)((next - prev) / 2);
}
