/*
 * A200263  a(n) = 1 iff n-th prime has an odd digit sum.
 * https://oeis.org/A200263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 iff the n-th prime has an odd digit sum
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/digit_sum.h"

/* Return 1 if the n‑th prime has an odd digit sum, else 0. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 */
        return (seq_t)0;

    /* fetch the n‑th prime (1‑based) */
    seq_t p = (seq_t)get_prime(n);

    /* compute its decimal digit sum */
    seq_t sum = digit_sum(p);

    /* odd digit sum ⇔ least‑significant bit is 1 */
    return (sum & (seq_t)1) ? (seq_t)1 : (seq_t)0;
}
