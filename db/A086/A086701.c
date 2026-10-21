/*
 * A086701  n-th prime number raised to the n-th composite number.
 * https://oeis.org/A086701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000040(n)^A002808(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/big_pow.h"

/* Return the n-th composite number (1‑based). */
static idx_t nth_composite(idx_t n)
{
    idx_t count = 0;
    idx_t num   = 4;                     /* composites start at 4 */
    while (1)
    {
        if (!is_prime(num))
        {
            ++count;
            if (count == n)
                return num;
        }
        ++num;
    }
}

/* OEIS sequence A086701. */
seq_t a(idx_t n)
{
    /* nth prime (offset 1) */
    seq_t prime = (seq_t)get_prime(n);

    /* nth composite (offset 1) */
    idx_t comp  = nth_composite(n);
    seq_t exp   = (seq_t)comp;

    /* compute prime^composite */
    return big_pow(prime, exp);
}
