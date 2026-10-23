/*
 * A032907  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i has
 *          d(0) <= d(1) >= d(2) <= ...
 * https://oeis.org/A032907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 5343, u32 n <= 5343, u64 n <= 5343,
 *            u128 n <= 5343, big any n
 * Helpers:   is_zigzag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Description: a(n) is the n-th non‑negative integer whose base‑10
 * digits satisfy d0 <= d1 >= d2 <= d3 >= … (starting with an
 * “up” step between the least significant and the next digit).
 * The function searches integers sequentially and returns the nth
 * such number.
 */
#include <stdbool.h>   /* for bool type */
#include "../utils/is_zigzag.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num   = 1;

    while (count < n)
    {
        if (is_zigzag(num)) {
            count++;
            if (count == n)
                return (seq_t)num;
        }
        num++;
    }
    /* Control should never reach here for n >= 1 */
    return 0;
}
