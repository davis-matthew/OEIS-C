/*
 * A001007  a(n) = ( Sum C(p,i); i=1,...,floor(2p/3) ) / p^2, where p =
 *          prime(n).
 * https://oeis.org/A001007
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Sum_{i=1..floor(2p/3)} C(p,i) ) / p^2, where p =
 *            prime(n). Each C(p,i) is held as q*p^2 + r with 0 <= r < p^2, so
 *            no intermediate value grows beyond about the size of the result
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   a001007_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a001007_nth_prime.h"

seq_t a(idx_t n) {
    uint64_t p = a001007_nth_prime((uint64_t)n);
    uint64_t P2 = p * p;
    uint64_t L = (2 * p) / 3;

    /* current binomial C(p,i) = q*P2 + r */
    seq_t q = 0;
    uint64_t r = p;
    seq_t Q = 0;
    uint64_t R = 0;

    for (uint64_t i = 1; i <= L; i++) {
        Q += q;
        R += r;
        if (R >= P2) { Q += (seq_t)(R / P2); R %= P2; }
        if (i < L) {
            uint64_t d = i + 1, m = p - i;
            uint64_t q0 = (uint64_t)(q % (seq_t)d);
            seq_t q1 = q / (seq_t)d;
            unsigned __int128 t = ((unsigned __int128)q0 * P2 + r) * m / d;
            q = q1 * (seq_t)m + (seq_t)(uint64_t)(t / P2);
            r = (uint64_t)(t % P2);
        }
    }
    return Q + (seq_t)(R / P2);
}
