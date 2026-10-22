/*
 * A003933  Order of universal Chevalley group B_4(q), q = prime power.
 * https://oeis.org/A003933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = B(A000961(n + 1), 4) where B(q, 4) = q^16 * (q^2 - 1)(q^4
 *            - 1)(q^6 - 1)(q^8 - 1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 7, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


static bool is_prime_power(uint64_t q) {
    if (q == 1) return true;
    
    for (uint64_t p = 2; p * p <= q; p++) {
        if (q % p == 0) {
            uint64_t temp = q;
            while (temp % p == 0) {
                temp /= p;
            }
            return temp == 1;
        }
    }
    
    return true;
}

static uint64_t prime_powers[10001];
static bool cache_initialized = false;

static void initialize_cache(void) {
    if (cache_initialized) return;
    
    uint64_t count = 0;
    uint64_t q = 1;
    while (count < 10001) {
        if (is_prime_power(q)) {
            prime_powers[count] = q;
            count++;
        }
        q++;
    }
    
    cache_initialized = true;
}

static uint64_t get_nth_prime_power(idx_t n) {
    if (n <= 10001) {
        initialize_cache();
        return prime_powers[n - 1];
    }
    
    uint64_t count = 0;
    uint64_t q = 1;
    while (true) {
        if (is_prime_power(q)) {
            count++;
            if (count == n) return q;
        }
        q++;
    }
}

seq_t a(idx_t n) {
    uint64_t q = get_nth_prime_power(n + 1);
    
    seq_t q_seq = (seq_t)q;
    seq_t q2 = q_seq * q_seq;
    seq_t q4 = q2 * q2;
    seq_t q6 = q4 * q2;
    seq_t q8 = q4 * q4;
    seq_t q16 = q8 * q8;
    
    seq_t result = q16;
    result *= (q2 - 1);
    result *= (q4 - 1);
    result *= (q6 - 1);
    result *= (q8 - 1);
    
    return result;
}
