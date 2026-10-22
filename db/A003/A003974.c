/*
 * A003974  Möbius transform of A003962.
 * https://oeis.org/A003974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(p^e) = ((q-1)/2)((q+1)/2)^(e-1) where q =
 *            nextPrime(p)
 * Range:     u8 n <= 508, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000248_pow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/a000248_pow.h"

static uint64_t next_prime(uint64_t p) {
    uint64_t q = p + 1;
    if (q == 2) return 2;
    if (q % 2 == 0) q++;
    while (!is_prime(q)) q += 2;
    return q;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    idx_t nn = n;
    seq_t result = 1;
    
    for (idx_t d = 2; d * d <= nn; d++) {
        if (nn % d == 0) {
            idx_t e = 0;
            while (nn % d == 0) {
                e++;
                nn /= d;
            }
            
            uint64_t q = next_prime(d);
            seq_t factor = (seq_t)((q - 1) / 2) * a000248_pow((seq_t)((q + 1) / 2), e - 1);
            result *= factor;
        }
    }
    
    if (nn > 1) {
        uint64_t q = next_prime(nn);
        seq_t factor = (seq_t)((q - 1) / 2) * a000248_pow((seq_t)((q + 1) / 2), 0);
        result *= factor;
    }
    
    return result;
}
