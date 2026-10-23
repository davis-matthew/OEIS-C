/*
 * A049613  a(n) = 2n - (largest prime < 2n-2).
 * https://oeis.org/A049613
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = 2n – (largest prime < 2n–2) Implementation: brute‑force
 *            search from 2n–3 downward, using the provided is_prime() routine
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* offset is 3 – behaviour for n < 3 is irrelevant */
    if (n < 3) {
        return 0;
    }

    idx_t target = 2*n - 2;          /* 2*n - 2 */
    for (idx_t x = target - 1; x >= 3; --x) {
        if (is_prime(x)) {
            return (seq_t)(2*n) - (seq_t)x;
        }
    }

    /* should never reach here; 2 is prime and 2 < target for n ≥ 3 */
    return 0;
}
