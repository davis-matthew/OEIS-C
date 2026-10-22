/*
 * A003643  Number of genera of Q(sqrt(-n)), n squarefree.
 * https://oeis.org/A003643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 2^omega(A005117(n)) if A005117(n) ≡ 1 (mod 4), else
 *            2^(omega(A005117(n)) - 1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   A001832_pow2, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A001832_pow2.h"
#include "../utils/count_distinct_primes.h"

static int is_squarefree(uint64_t n) {
    if (n <= 1) return n == 1;
    
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;
        }
    }
    return 1;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t k = 1;
    while (count < n) {
        if (is_squarefree(k)) count++;
        if (count < n) k++;
    }
    
    int omega = count_distinct_primes(k);
    int exponent = omega;
    if (k % 4 != 1) {
        exponent = omega - 1;
    }
    if (exponent < 0) exponent = 0;
    
    return A001832_pow2((idx_t)exponent);
}
