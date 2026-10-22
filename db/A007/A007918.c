/*
 * A007918  Least prime >= n (version 1 of the "next prime" function).
 * https://oeis.org/A007918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = least prime >= n; found by searching upward from max(n,2)
 *            with trial division
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a7918_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a7918_isprime.h"

seq_t a(idx_t n) {
    uint64_t c = (uint64_t)n;
    if (c < 2) c = 2;
    while (!a7918_isprime(c)) c++;
    return (seq_t)c;
}
