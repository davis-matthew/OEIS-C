/*
 * A094815  (prime(prime(n)))^n.
 * https://oeis.org/A094815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(prime(n)))^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Offset 1: n >= 1 */
    uint32_t p1 = get_prime((idx_t)n);          /* p1 = prime(n)   */
    uint32_t p2 = get_prime((idx_t)p1);         /* p2 = prime(p1)  */
    seq_t base = (seq_t)p2;
    return big_pow(base, (seq_t)n);
}
