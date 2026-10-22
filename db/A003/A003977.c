/*
 * A003977  Inverse Möbius transform of A003963.
 * https://oeis.org/A003977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 629, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: a(2^e) = e+1; a(p^e) = (q^(e+1)-1)/(q-1) where q=pi(p) for p>2 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

static seq_t count_primes_up_to(uint64_t n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    seq_t count = 1;
    for (uint64_t i = 3; i <= n; i += 2) {
        if (is_prime(i)) count++;
    }
    return count;
}

static seq_t geometric_sum(seq_t q, idx_t e) {
    seq_t result = 1;
    seq_t power = q;
    for (idx_t i = 1; i <= e; i++) {
        result += power;
        power *= q;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    uint64_t m = n;
    
    if (m % 2 == 0) {
        idx_t e = 0;
        while (m % 2 == 0) {
            e++;
            m /= 2;
        }
        result *= (e + 1);
    }
    
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            seq_t q = count_primes_up_to(p);
            result *= geometric_sum(q, e);
        }
    }
    
    if (m > 1) {
        seq_t contribution;
        if (m == 2) {
            contribution = 2;
        } else {
            seq_t q = count_primes_up_to(m);
            contribution = geometric_sum(q, 1);
        }
        result *= contribution;
    }
    
    return result;
}
