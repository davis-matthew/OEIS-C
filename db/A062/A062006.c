/*
 * A062006  a(n) = prime(n)^n + 1.
 * https://oeis.org/A062006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = prime(n)^n + 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* n < 1 is outside the defined offset */
        return 0;

    /* nth prime (offset 1) */
    uint32_t p = get_prime(n);

    /* compute p^n */
    seq_t val = big_pow((seq_t)p, (seq_t)n);

    /* finally: a(n) = p^n + 1 */
    return val + 1;
}
