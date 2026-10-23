/*
 * A104048  Numbers k such that k11 is prime and k is a multiple of ten.
 * https://oeis.org/A104048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th multiple of 10 such that 100*k + 11 is prime
 *            (Offset 1, so a(1) = 0)
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safeguard for offset, though n≥1 in the tests */

    idx_t count = 0;
    for (idx_t k = 0; ; k += 10) {
        uint64_t cand = (uint64_t)k * 100ULL + 11ULL;
        if (is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
