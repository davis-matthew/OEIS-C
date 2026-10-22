/*
 * A003854  Order of simple Chevalley group D_8(q), q = prime power.
 * https://oeis.org/A003854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(A000961(n+1),8) where d(q,8) =
 *            q^56*(q^8-1)*Prod_{i=1..7}(q^(2i)-1)/gcd(4,q^8-1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 1, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime_power_u64(uint64_t n) {
    uint64_t p;
    if (n < 2) return n == 1;
    for (p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            return n == 1;
        }
    }
    return true;
}

static seq_t pow_seq(seq_t b, unsigned e) {
    seq_t r = 1;
    while (e--) r *= b;
    return r;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t q_idx = 0;
    seq_t q, result, t;
    unsigned i;

    while (count < n + 1) {
        q_idx++;
        if (is_prime_power_u64(q_idx)) count++;
    }

    q = (seq_t)q_idx;
    t = pow_seq(q, 8) - 1;
    if (q_idx % 2 == 1) t /= 4;
    result = pow_seq(q, 56) * t;
    for (i = 1; i <= 7; i++) {
        result *= pow_seq(q, 2 * i) - 1;
    }
    return result;
}
