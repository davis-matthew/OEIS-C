/*
 * A216600  Number of ways to write n as a product with all factors <= n/4.
 * https://oeis.org/A216600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of unordered factorizations of n into factors all ≤
 *            floor(n/4) (at least two factors). The depth‑first search
 *            enumerates all such factorizations using ascending factor order
 *            to avoid duplicates
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_fact.h"

seq_t a(idx_t n)
{
    idx_t max_factor = n / 4;
    if (max_factor < 2) return 0;
    return count_fact(n, 2, max_factor);
}
