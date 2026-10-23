/*
 * A155480  Composites n with identical largest digits of n and n-th prime.
 * https://oeis.org/A155480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155480: composites n with identical largest digits of n and n‑th prime. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

/* Return largest decimal digit of x. */
static int largest_digit(seq_t x)
{
    int maxd = 0;
    while (x > 0)
    {
        seq_t d = x % 10;
        int di = (int)(unsigned long long)d;
        if (di > maxd) maxd = di;
        x /= 10;
    }
    return maxd;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for zero index */

    idx_t count = 0;
    idx_t m = 4;   /* first composite number */

    for (;; ++m)
    {
        if (!is_prime(m))
        {
            int d_m = largest_digit((seq_t)m);
            uint32_t p = get_prime((idx_t)m);
            int d_p = largest_digit((seq_t)p);
            if (d_m == d_p)
            {
                ++count;
                if (count == n)
                    return (seq_t)m;
            }
        }
    }
}
