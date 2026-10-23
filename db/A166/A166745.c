/*
 * A166745  a(n) is the concatenation, in ascending order, of the set of
 *          digits 1,2,4,8 whose sum equals the n-th prime using a minimal
 *          number of digits.
 * https://oeis.org/A166745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   For the n‑th prime p, express p as a sum of the fewest digits
 *            from the set {1,2,4,8}. The resulting digits are concatenated in
 *            ascending order
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑based). */
    uint32_t prime = get_prime(n);

    /* Greedy decomposition – optimal for powers of two. */
    idx_t cnt8 = (idx_t)(prime / 8);
    uint64_t r = (uint64_t)prime % 8;
    idx_t cnt4 = (idx_t)(r / 4);
    r %= 4;
    idx_t cnt2 = (idx_t)(r / 2);
    idx_t cnt1 = (idx_t)(r % 2);

    /* Build the concatenated number in ascending digit order. */
    seq_t result = 0;
    for (idx_t i = 0; i < cnt1; ++i) result = result * 10 + 1;
    for (idx_t i = 0; i < cnt2; ++i) result = result * 10 + 2;
    for (idx_t i = 0; i < cnt4; ++i) result = result * 10 + 4;
    for (idx_t i = 0; i < cnt8; ++i) result = result * 10 + 8;
    return result;
}
