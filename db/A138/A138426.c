/*
 * A138426  a(n) = ((prime(n))^5-prime(n))/5.
 * https://oeis.org/A138426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((prime(n))^5 - prime(n))/5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 30, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n) {
    seq_t p = (seq_t)get_prime(n);

#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value) {
        __int128 v = (__int128)p;
        v *= (__int128)p;
        v *= (__int128)p;
        v *= (__int128)p;
        v *= (__int128)p;
        return (seq_t)((v - (__int128)p)/5);
    } else {
        seq_t v = p * p * p * p * p;
        return (v - p)/5;
    }
#else
    __int128 v = (__int128)p * (__int128)p * (__int128)p * (__int128)p * (__int128)p;
    return (seq_t)((v - (__int128)p)/5);
#endif
}
