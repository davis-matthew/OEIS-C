/*
 * A176266  Binomial(prime(n),s)/prime(n) where s is the sum of the decimal
 *          digits of n.
 * https://oeis.org/A176266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binomial(prime(n),digit_sum(n))/prime(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 16, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binom, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/binom.h"

/* Sum of decimal digits of n, where n fits in idx_t (holds 64 bits) */
static idx_t digit_sum_idx(idx_t n)
{
    idx_t sum = (idx_t)0;
    while (n != (idx_t)0) {
        sum += n % (idx_t)10;
        n   /= (idx_t)10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;       /* undefined for n < 1 */

    /* nth prime */
    idx_t p = (idx_t)get_prime(n);

    /* sum of decimal digits of n */
    idx_t s = digit_sum_idx(n);

    /* binomial(p, s) */
    seq_t b = binom(p, s);

    /* division by prime p */
    return b / (seq_t)p;
}
