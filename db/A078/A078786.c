/*
 * A078786  Period of cycle of the inventory sequence (as in A063850) starting
 *          with n.
 * https://oeis.org/A078786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nice, nonn
 * Formula:   a(n) = period of the cycle of the inventory sequence starting
 *            from n (found by cycle detection)
 * Range:     u8 none, u16 none, u32 n <= 39, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   inventory
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/inventory.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;

    /* Floyd's cycle-finding algorithm */
    seq_t tortoise = inventory(cur);
    seq_t hare = inventory(inventory(cur));

    while (tortoise != hare) {
        tortoise = inventory(tortoise);
        hare = inventory(inventory(hare));
    }

    /* determine cycle length (lambda) */
    seq_t lambda = 1;
    hare = inventory(tortoise);
    while (hare != tortoise) {
        hare = inventory(hare);
        ++lambda;
    }

    return lambda;
}
