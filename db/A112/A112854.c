/*
 * A112854  Primes of the form (2*n)!/n!-1.
 * https://oeis.org/A112854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime( (2*m)!/m! - 1 ) with offset 1 (i.e. a(n) is the
 *            nth prime among the numbers (2*m)!/m! - 1, where m runs through
 *            1,2,3,... )
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime, isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/isprime_big.h"

static inline bool term_is_prime(seq_t x) {
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        return is_prime((uint64_t)x);
    } else {
        return isprime_big(x);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* unused, offset 1 */
    seq_t P = (seq_t)2;            /* P_1 = product_{i=1}^1 (1+i) = 2 */
    idx_t m = 1;                   /* current m (i.e., n in the definition) */
    idx_t count = 0;

    while (true) {
        seq_t term = P - 1;        /* current candidate */
        if (term_is_prime(term)) {
            ++count;
            if (count == n) return term;
        }

        /* advance to next m: compute P_{m+1} from P_m */
        ++m;                                   /* now m = old m + 1 */
        seq_t divisor = (seq_t)(m);
        seq_t factor1 = (seq_t)(2 * (m - 1) + 1); /* 2*(prev m)+1 */
        seq_t factor2 = (seq_t)(2 * (m - 1) + 2); /* 2*(prev m)+2 */
        P = (P / divisor) * factor1 * factor2;
    }
}
