/*
 * A003980  Möbius transform of A003965.
 * https://oeis.org/A003980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative with a(p^e) = (q-1)*q^(e-1) where q =
 *            Fib(pi(p)+2)
 * Range:     u8 n <= 36, u16 n <= 82, u32 n <= 198, u64 n <= 478,
 *            u128 n <= 1102, big any n
 * Helpers:   fib, prime_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/fib.h"
#include "../utils/prime_count.h"

static seq_t power(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t m = n;
    
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            
            idx_t pi_p = prime_count(p);
            seq_t q = fib(pi_p + 2);
            seq_t factor = (q - 1) * power(q, e - 1);
            result *= factor;
        }
    }
    
    if (m > 1) {
        idx_t pi_m = prime_count(m);
        seq_t q = fib(pi_m + 2);
        result *= (q - 1);
    }
    
    return result;
}
