/*
 * A171052  In the sequence of prime numbers, replace all the '9' digits with
 *          '3' and vice versa.
 * https://oeis.org/A171052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = swap digits 3↔9 of the n-th prime number (OEIS A171052)
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 995 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t swap_3_9(seq_t num)
{
    seq_t res = 0;
    seq_t pow = 1;
    while (num > 0)
    {
        uint8_t d = (uint8_t)(num % 10);
        uint8_t nd = (d == 3 ? 9 : d == 9 ? 3 : d);
        res += (seq_t)nd * pow;
        num /= 10;
        pow *= 10;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)                      /* out of offset */
        return (seq_t)0;

    idx_t count = 0;
    idx_t candidate = 2;

    /* Find the n‑th prime */
    while (1)
    {
        if (is_prime(candidate))
        {
            ++count;
            if (count == n)
                return swap_3_9((seq_t)candidate);
        }
        /* Skip even numbers >2 for a tiny speedup */
        if (candidate == 2)
            candidate = 3;
        else
            ++candidate;
    }
}
