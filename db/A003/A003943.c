/*
 * A003943  Order of simple Chevalley group B_7(q), q = prime power.
 * https://oeis.org/A003943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^49 * Prod_{i=1..7} (q^(2i) - 1) / gcd(2, q-1), where q
 *            = A000961(n+1) is the n-th prime power > 1
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 1, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime_power_u64(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            return n == 1;
        }
    }
    return true;
}

/* k-th prime power > 1 (k >= 1): 2, 3, 4, 5, 7, 8, 9, ... */
static uint64_t kth_prime_power(idx_t k) {
    idx_t count = 0;
    for (uint64_t i = 2; ; i++) {
        if (is_prime_power_u64(i)) {
            count++;
            if (count == k) return i;
        }
    }
}

static seq_t power_seq(seq_t base, idx_t e) {
    seq_t result = 1;
    seq_t b = base;
    while (e > 0) {
        if (e & 1) result *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t q = kth_prime_power(n);
    seq_t qs = (seq_t)q;
    seq_t q2 = qs * qs;
    seq_t result = power_seq(qs, 49);
    seq_t qp = 1;
    for (idx_t i = 1; i <= 7; i++) {
        qp *= q2;
        seq_t f = qp - 1;
        if (i == 1 && (q & 1)) f /= 2; /* gcd(2, q-1) = 2 for odd q */
        result *= f;
    }
    return result;
}
