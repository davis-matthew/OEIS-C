/*
 * A176815  Radius of the second-shortest interval (of positive length)
 *          centered at prime(n) that has prime endpoints, (zero if no
 *          solution exists).
 * https://oeis.org/A176815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176815:
 * Radius of the second-shortest interval centered at the n-th prime
 * that has prime endpoints; zero if no such interval exists. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* n‑th prime (1‑based) */
    uint32_t prim = get_prime(n);
    uint64_t p = prim;

    seq_t first  = 0;  /* radius of the first valid interval */
    seq_t second = 0;  /* radius of the second valid interval (answer) */

    for (uint64_t r = 1; ; ++r)
    {
        /* left endpoint becomes < 2 when r >= p‑1 */
        if (p <= r + 1)
            break;

        uint64_t left  = p - r;
        uint64_t right = p + r;

        if (is_prime((idx_t)left) && is_prime((idx_t)right))
        {
            if (first == 0)
                first = (seq_t)r;
            else
            {
                second = (seq_t)r;
                break;
            }
        }
    }

    return second;  /* zero if a second interval does not exist */
}
