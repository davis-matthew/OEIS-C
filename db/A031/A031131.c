/*
 * A031131  Difference between n-th prime and (n+2)-nd prime.
 * https://oeis.org/A031131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(n+2) - A000040(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }
    uint32_t p_n  = get_prime(n);       /* the n‑th prime */
    uint32_t p_n2 = get_prime(n + 2);   /* the (n+2)‑nd prime */
    return (seq_t)(p_n2 - p_n);
}
