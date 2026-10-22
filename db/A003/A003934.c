/*
 * A003934  Order of universal Chevalley group B_5(q), q = prime power.
 * https://oeis.org/A003934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^25 * Product_{i=1..5} (q^(2i) - 1), where q =
 *            A000961(n+1) is the n-th prime power > 1
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 4, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool is_prime_power_u64(uint64_t n) {
    uint64_t p;
    if (n < 2) return false;
    for (p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            return n == 1;
        }
    }
    return true;
}

/* k-th prime power > 1 (k >= 1): 2, 3, 4, 5, 7, 8, 9, ... */
static uint64_t nth_prime_power(idx_t k) {
    static uint64_t *cache = NULL;
    static idx_t count = 0, cap = 0;
    static uint64_t next = 2;
    while (count < k) {
        if (is_prime_power_u64(next)) {
            if (count >= cap) {
                cap = cap ? cap * 2 : 256;
                cache = (uint64_t *)realloc(cache, (size_t)cap * sizeof(uint64_t));
            }
            cache[count++] = next;
        }
        next++;
    }
    return cache[k - 1];
}

static seq_t pow_seq(seq_t b, int e) {
    seq_t r = 1;
    while (e-- > 0) r *= b;
    return r;
}

seq_t a(idx_t n) {
    seq_t q = (seq_t)nth_prime_power(n);
    seq_t q2 = q * q;
    seq_t result = pow_seq(q, 25);
    seq_t p = 1;
    int i;
    for (i = 1; i <= 5; i++) {
        p *= q2;
        result *= (p - 1);
    }
    return result;
}
