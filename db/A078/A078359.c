/*
 * A078359  Number of ways to write n as sum of a positive square and a
 *          positive cube.
 * https://oeis.org/A078359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (s > 0, c > 0) : n = s^2 + c^3 }
 * Range:     u8 n <= 33, u16 n <= 8512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_square_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_square_seq.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t c = 1; ; ++c) {
        seq_t cube = (seq_t)c * (seq_t)c * (seq_t)c;
        if (cube >= (seq_t)n)
            break;
        seq_t rest = (seq_t)n - cube;
        if (is_square_seq(rest))
            ++count;
    }
    return count;
}
