/*
 * A003941  Order of simple Chevalley group B_5(q), q = prime power.
 * https://oeis.org/A003941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(A000961(n+1), 5) with b(q,5) = q^25 *
 *            (q^2-1)(q^4-1)(q^6-1)(q^8-1)(q^10-1) / gcd(2, q-1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 4, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_prime_power(uint64_t n) {
    if (n <= 1) return false;
    if (n % 2 == 0) return (n & (n - 1)) == 0;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            while (n % d == 0) n /= d;
            return n == 1;
        }
    }
    return true;
}

/* A000961(n+1) = n-th prime power (A000961 starts with 1) */
static uint64_t nth_prime_power(uint64_t n) {
    uint64_t count = 0, num = 1;
    while (count < n) {
        num++;
        if (is_prime_power(num)) count++;
    }
    return num;
}

seq_t a(idx_t n) {
    uint64_t q = nth_prime_power(n);
    seq_t qs = (seq_t)q;
    seq_t q2 = qs * qs;
    seq_t result = 1;
    seq_t qp = 1;
    int k;
    for (k = 1; k <= 5; k++) {
        seq_t f;
        qp *= q2;              /* q^(2k) */
        f = qp - 1;
        if (k == 1 && (q & 1)) f /= 2;   /* gcd(2, q-1) = 2 for odd q */
        result *= f;
    }
    for (k = 0; k < 25; k++) result *= qs;
    return result;
}
