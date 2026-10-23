/*
 * A194606  Least k >= 0 such that prime(n)*2^k - 1 or prime(n)*2^k + 1 is
 *          prime, or -1 if no such value exists, where prime(n) denotes the
 *          n-th prime number.
 * https://oeis.org/A194606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k >= 0 such that prime(n)*2^k - 1 or prime(n)*2^k +
 *            1 is prime, -1 if no such k exists
 * Range:     i8 n <= 13, i16 n <= 52, i32 n <= 52, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types */
#include <stdbool.h>
/* Utility functions */
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Maximum k to search before giving up and returning -1.  */
#define MAX_K 2000

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;              /* offset starts at 1 */

    uint32_t p = get_prime(n);          /* the n‑th prime */
    seq_t power = 1;                    /* 2^k, initially k=0 */

    for (idx_t k = 0; k <= MAX_K; ++k) {
        seq_t mult = (seq_t)p * power;  /* p * 2^k */
        seq_t a_val = mult - 1;
        seq_t b_val = mult + 1;

        if (is_prime_big(a_val) || is_prime_big(b_val))
            return (seq_t)k;            /* found the smallest k */

        power <<= 1;                    /* multiply power by 2 for next k */
    }

    return (seq_t)-1;                    /* no such k within limit */
}
