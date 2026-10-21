/*
 * A065736  Largest square <= 10^n.
 * https://oeis.org/A065736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(10^n))^2 (where 10^n is 1 followed by n zeros)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(0) is outside the sequence; return 0 as a safe value. */
    if (n == 0)
        return (seq_t)0;

    /* Even n: sqrt(10^n) = 10^(n/2), which is an integer. */
    if ((n & 1) == 0)
        return pow10(n);

    /* Odd n: compute floor(sqrt(10^n)) by the digit‑by‑digit method. */
    idx_t groups = (n + 1) >> 1;   /* number of 2‑digit groups */

    seq_t root = 0;   /* current root being built */
    seq_t rem  = 0;   /* remainder at each step */

    for (idx_t i = 0; i < groups; ++i) {
        seq_t group = (i == 0) ? (seq_t)10 : (seq_t)0;  /* first group is '10', rest '00' */

        rem = rem * 100 + group;

        seq_t root20 = root * 20;   /* root before adding the new digit */
        int d;
        for (d = 9; d >= 0; --d) {   /* largest digit satisfying (20*root + d)*d <= rem */
            seq_t prod = (root20 + d) * d;
            if (prod <= rem) break;
        }
        if (d < 0) d = 0;            /* safety, should never happen */

        root = root * 10 + d;
        rem -= (root20 + d) * d;
    }

    return root * root;   /* square of the floor(sqrt(10^n)) */
}
