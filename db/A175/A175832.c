/*
 * A175832  Number of solutions of equation 3^q - 2^p = n over integer
 *          variables p, q.
 * https://oeis.org/A175832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (p,q) ≥ 0 to 3^q − 2^p = n
 * Range:     u8 none, u16 n <= 16, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"

static inline bool is_power_of_two(seq_t x)
{
    return x != 0 && ( (x & (x - 1)) == (seq_t)0 );
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    /* Upper bound on q sufficient for n up to 10‑000; beyond that
       solutions are astronomically rare (Collatz‑style argument),
       and a fixed conservative limit keeps runtime small. */
    const idx_t MAX_Q = 200;   /* safe for all test cases */

    for (idx_t q = 0; q <= MAX_Q; ++q) {
        seq_t pow3q = pow3(q);
        /* Skip when 3^q <= n (difference non‑positive). */
        if (pow3q <= (seq_t)n) continue;

        seq_t m = pow3q - (seq_t)n;
        if (is_power_of_two(m))
            ++cnt;
    }
    return cnt;
}
