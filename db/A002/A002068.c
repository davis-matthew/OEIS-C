/*
 * A002068  Wilson remainders: a(n) = ((p-1)!+1)/p mod p, where p = prime(n).
 * https://oeis.org/A002068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = ((p-1)! + 1)/p mod p, where p = prime(n); computed as
 *            ((p-1)! mod p^2 + 1)/p mod p
 * Range:     u8 n <= 59, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   a2068_isprime, a2068_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2068_isprime.h"
#include "../utils/a2068_nth_prime.h"

seq_t a(idx_t n) {
    uint64_t p = a2068_nth_prime(n);
    if (p == 2) return (seq_t)1;
    unsigned __int128 p2 = (unsigned __int128)p * p;
    unsigned __int128 fact = 1;
    for (uint64_t i = 2; i < p; i++)
        fact = (fact * i) % p2;
    uint64_t q = (uint64_t)(((fact + 1) / p) % p);
    return (seq_t)q;
}
