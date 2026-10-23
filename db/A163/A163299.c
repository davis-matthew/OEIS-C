/*
 * A163299  a(n) = (the number of divisors of n)^(the binary order of n).
 * https://oeis.org/A163299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = tau(n) ^ ceil(log2(n))
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 167, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, divisor_count_u64, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(n) is the number of divisors of n (A000005).  */
/* ceil(log2(n)) is the binary order of n (A029837).   */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/floor_log2_uint64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Number of divisors of n. */
    seq_t d = divisor_count_u64((uint64_t)n);

    /* Binary order of n.  */
    unsigned bits = floor_log2_uint64(n);
    unsigned exp  = (((uint64_t)1 << bits) == n) ? bits : bits + 1;

    /* Compute d ^ exp exactly. */
    return big_pow(d, (seq_t)exp);
}
