/*
 * A006556  Number of different cycles of digits in the decimal expansions of
 *          1/p, 2/p, ..., (p-1)/p where p = n-th prime different from 2 or 5.
 * https://oeis.org/A006556
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = (p-1)/x, where p is the (n-2)-th prime != 2,5 and x =
 *            ord_p(10)
 * Range:     u8 n <= 627, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"


static uint64_t nth_prime_excluding_2_5(uint64_t n) {
    uint64_t count = 0;
    uint64_t candidate = 3;
    
    while (true) {
        if (is_prime(candidate) && candidate != 2 && candidate != 5) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        candidate += 2;
    }
}

static uint64_t multiplicative_order(uint64_t a, uint64_t p) {
    uint64_t power = a % p;
    uint64_t order = 1;
    
    while (power != 1) {
        power = (power * a) % p;
        order++;
    }
    
    return order;
}

seq_t a(idx_t n) {
    uint64_t prime = nth_prime_excluding_2_5(n - 2);
    uint64_t order = multiplicative_order(10, prime);
    return (seq_t)((prime - 1) / order);
}
