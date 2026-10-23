/*
 * A030002  (prime(n)-5)(prime(n)-7)(prime(n)-9)/48.
 * https://oeis.org/A030002
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-5)*(prime(n)-7)*(prime(n)-9)/48
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 262, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 3 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n < 3u) return 0;
    seq_t p = (seq_t)get_prime(n);
    seq_t t1 = p - 5;
    seq_t t2 = p - 7;
    seq_t t3 = p - 9;
    seq_t prod = t1 * t2;
    prod *= t3;
    return prod / 48;
}
