/*
 * A003935  Order of universal Chevalley group B_6(q), q = prime power.
 * https://oeis.org/A003935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^36 * Product_{i=1..6} (q^(2i) - 1), where q =
 *            A000961(n+1) is the n-th prime power > 1
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool is_prime_power_u64(uint64_t m) {
    uint64_t p = 0, i;
    if (m < 2) return false;
    for (i = 2; i * i <= m; i++) {
        if (m % i == 0) { p = i; break; }
    }
    if (p == 0) return true;
    while (m % p == 0) m /= p;
    return m == 1;
}

static uint64_t nth_prime_power(idx_t k) {
    static uint64_t *cache = NULL;
    static idx_t count = 0, cap = 0;
    uint64_t i;
    if (k <= count) return cache[k - 1];
    i = count ? cache[count - 1] + 1 : 2;
    while (count < k) {
        if (is_prime_power_u64(i)) {
            if (count == cap) {
                idx_t nc = cap ? cap * 2 : 1024;
                cache = (uint64_t *)realloc(cache, nc * sizeof(uint64_t));
                cap = nc;
            }
            cache[count++] = i;
        }
        i++;
    }
    return cache[k - 1];
}

static seq_t pow_seq(seq_t base, uint64_t e) {
    seq_t r = 1, b = base;
    while (e > 0) {
        if (e & 1) r = r * b;
        e >>= 1;
        if (e) b = b * b;
    }
    return r;
}

seq_t a(idx_t n) {
    uint64_t q = nth_prime_power(n);
    seq_t qs = (seq_t)q;
    seq_t q2 = qs * qs;
    seq_t pw = 1;
    seq_t result = pow_seq(qs, 36);
    int i;
    for (i = 1; i <= 6; i++) {
        pw = pw * q2;
        result = result * (pw - 1);
    }
    return result;
}
