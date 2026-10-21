/*
 * A087990  Number of palindromic divisors of n.
 * https://oeis.org/A087990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n that are palindromic in decimal
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_pal_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/is_pal_uint64.h"

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    if (n == 0)          /* safety for out‑of‑range calls */
        return 0;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pc = 0;
    factor_u64((uint64_t)n, primes, exps, &pc);

    /* Enumerate divisors and count palindromic ones */
    seq_t   count = 0;
    if (pc == 0) {       /* n == 1 */
        return 1;
    }

    size_t cur[64] = {0};

    for (;;) {
        uint64_t d = 1;
        for (size_t i = 0; i < pc; ++i) {
            uint64_t p = primes[i];
            for (size_t k = 0; k < cur[i]; ++k)
                d *= p;
        }
        if (is_pal_uint64(d))
            ++count;

        /* Increment exponent vector (Cartesian product) */
        size_t pos = 0;
        while (pos < pc) {
            if (++cur[pos] <= (size_t)exps[pos])
                break;
            cur[pos] = 0;
            ++pos;
        }
        if (pos == pc)   /* finished all combinations */
            break;
    }

    return count;
}
