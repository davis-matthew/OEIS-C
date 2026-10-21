/*
 * A092909  Interpolate 0's between each pair of digits of n-th prime.
 * https://oeis.org/A092909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpolate 0 between digits of the n-th prime (OEIS
 *            A092909)
 * Range:     u8 n <= 10, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"        /* uint32_t get_prime(idx_t k); */
#include "../utils/digit_len.h"        /* int digit_len(seq_t x); */
#include "../utils/pow10.h"           /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based).  It fits in a 32‑bit unsigned integer for n <= 65536.  */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Number of decimal digits of the prime. */
    int len = digit_len(p);
    if (len <= 0) return (seq_t)0;   /* safety, should not happen */

    /* Prepare to extract digits from most significant to least.  */
    seq_t factor = pow10((idx_t)(len - 1));
    seq_t result = 0;

    for (int i = 0; i < len; ++i) {
        seq_t d = p / factor;            /* current digit */
        result = result * 10 + d;        /* append digit */

        if (i < len - 1)
            result = result * 10;         /* insert 0 */

        p = p - d * factor;              /* remaining part */
        factor = factor / 10;             /* move to next digit */
    }
    return result;
}
