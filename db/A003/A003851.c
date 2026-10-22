/*
 * A003851  Order of simple Chevalley group D_5(q), q = prime power.
 * https://oeis.org/A003851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            q^20*(q^2-1)*(q^4-1)*(q^6-1)*(q^8-1)*(q^5-1)/gcd(4,q^5-1), q =
 *            A000961(n+1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 5, big any n
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

/* A000961(m): m-th term of 1, 2, 3, 4, 5, 7, 8, 9, ... */
static uint64_t nth_prime_power(idx_t m) {
    if (m <= 1) return 1;
    idx_t count = 1;
    uint64_t q = 1;
    while (count < m) {
        q++;
        if (is_prime_power_u64(q)) count++;
    }
    return q;
}

seq_t a(idx_t n) {
    uint64_t qn = nth_prime_power(n + 1);
    unsigned d;
    if (qn % 2 == 0) d = 1;
    else if (qn % 4 == 1) d = 4;
    else d = 2;

    seq_t q = (seq_t)qn;
    seq_t q2 = q * q;
    seq_t q4 = q2 * q2;
    seq_t q5 = q4 * q;
    seq_t q6 = q4 * q2;
    seq_t q8 = q4 * q4;
    seq_t q20 = q8 * q8 * q4;

    seq_t r = (q5 - 1) / d;
    r *= q2 - 1;
    r *= q4 - 1;
    r *= q6 - 1;
    r *= q8 - 1;
    r *= q20;
    return r;
}
