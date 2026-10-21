/*
 * A228432  Sum_{i=1..floor(prime(n)/4)} floor(sqrt(i*prime(n))).
 * https://oeis.org/A228432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor(prime(n)/4)} floor(sqrt(i*prime(n)))
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* obtain the n‑th prime and work with seq_t */
    uint32_t p_u = get_prime(n);
    seq_t prime = (seq_t)p_u;
    seq_t limit = prime / 4;          /* floor(p/4) */

    seq_t sum = 0;
    for (seq_t i = 1; i <= limit; ++i) {
        seq_t prod = i * prime;
        seq_t root = floor_sqrt(prod);
        sum += root;
    }
    return sum;
}
