/*
 * A016016  Number of iterations of Reverse and Add which lead to a
 *          palindrome, or -1 if no palindrome is ever reached.
 * https://oeis.org/A016016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 88, u32 n <= 88, u64 n <= 195,
 *            u128 n <= 195, big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of Reverse‑and‑Add iterations needed to obtain a palindrome,
 * starting from the integer n (1‑based).  If no palindrome is reached within
 * a reasonable number of iterations the function returns -1.
 *
 * The algorithm performs at least one Reverse‑and‑Add operation even when
 * n is already a palindrome, in accordance with the OEIS definition.
 */

#include <stdbool.h>
#include "../utils/reverse_decimal.h"
#include "../utils/is_palindrome.h"

#define MAX_ITER 10000   /* Optional upper bound to avoid infinite loops */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    int steps = 0;

    while (steps < MAX_ITER) {
        ++steps;                       /* perform one Reverse‑and‑Add step */
        cur = cur + reverse_decimal(cur);
        if (is_palindrome(cur, 10))
            return (seq_t)steps;       /* palindrome reached */
    }
    /* No palindrome within MAX_ITER steps: conventionally return -1. */
    return (seq_t)-1;
}
