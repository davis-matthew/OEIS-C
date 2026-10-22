/*
 * A002815  a(n) = n + Sum_{k=1..n} pi(k), where pi() = A000720.
 * https://oeis.org/A002815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n + Sum_{k=1..n} pi(k), where pi(k) is the prime counting
 *            function
 * Range:     u8 n <= 33, u16 n <= 824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static bool is_prime(seq_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    for (seq_t d = 3; d * d <= p; d += 2) {
        if (p % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t result = (seq_t)n;
    seq_t prime_count = 0;
    
    for (seq_t k = 1; k <= (seq_t)n; k++) {
        if (is_prime(k)) prime_count++;
        result += prime_count;
    }
    
    return result;
}
