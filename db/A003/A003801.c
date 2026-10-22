/*
 * A003801  Order of universal Chevalley group A_3 (q) (or D_3 (q)), q = prime
 *          power.
 * https://oeis.org/A003801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^6 * (q^2 - 1) * (q^3 - 1) * (q^4 - 1), where q =
 *            A000961(n+1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_power(uint64_t n) {
    if (n < 2) return false;
    
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            uint64_t temp = n;
            while (temp % p == 0) {
                temp /= p;
            }
            return temp == 1;
        }
    }
    
    return true;
}

static uint64_t a000961(idx_t n) {
    if (n == 1) return 1;
    
    uint64_t count = 1;
    for (uint64_t i = 2; ; i++) {
        if (is_prime_power(i)) {
            count++;
            if (count == n) return i;
        }
    }
}

seq_t a(idx_t n) {
    uint64_t q = a000961(n + 1);
    seq_t q_seq = (seq_t)q;
    
    seq_t q2 = q_seq * q_seq;
    seq_t q3 = q2 * q_seq;
    seq_t q4 = q3 * q_seq;
    seq_t q6 = q4 * q2;
    
    seq_t result = q6 * (q2 - 1) * (q3 - 1) * (q4 - 1);
    return result;
}
