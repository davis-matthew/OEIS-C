/*
 * A070562  Fecundity of n.
 * https://oeis.org/A070562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of iterations needed to reach a zero‑digit number
 *            when starting at n and repeatedly replacing x by x + prod of its
 *            decimal digits (stop when the product is 0). See OEIS A070562,
 *            “Fecundity of n”
 * Range:     u8 n <= 426, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if needed */
#include "../utils/digit_product.h"

seq_t a(idx_t n) {
    seq_t x = n;
    seq_t steps = 0;
    while (true) {
        seq_t p = digit_product(x);
        if (p == 0) break;            /* product zero => terminate */
        x += p;
        steps++;
    }
    return steps;
}
