/*
 * A163299  a(n) = (the number of divisors of n)^(the binary order of n).
 * https://oeis.org/A163299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = tau(n) ^ ceil(log2(n))
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 167, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 970 (2026-11-01)
 * Helpers:   ilog2, ipow, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(n) is the number of divisors of n (A000005).  */
/* ceil(log2(n)) is the binary order of n (A029837).   */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ilog2.h"
#include "../utils/ipow.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* Number of divisors of n. */
    seq_t d = num_divisors((uint64_t)n);

    /* Binary order of n.  */
    unsigned bits = ilog2(n);
    unsigned exp  = (((uint64_t)1 << bits) == n) ? bits : bits + 1;

    /* Compute d ^ exp exactly. */
    return ipow(d, (seq_t)exp);
}
