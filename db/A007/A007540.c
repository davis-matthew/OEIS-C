/*
 * A007540  Wilson primes: primes p such that (p-1)! == -1 (mod p^2).
 * https://oeis.org/A007540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more, bref, nice
 * Formula:   search: a(n) is the n-th prime p with (p-1)! == -1 (mod p^2),
 *            found by testing primes in increasing order
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   a007540_is_wilson, a007540_isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a007540_is_wilson.h"
#include "../utils/a007540_isprime.h"

seq_t a(idx_t n)
{
    static uint64_t found[16];
    static idx_t nfound = 0;
    static uint64_t next_p = 2;

    while (nfound < n) {
        uint64_t p = next_p++;
        if (a007540_isprime(p) && a007540_is_wilson(p)) {
            if (nfound < 16) found[nfound] = p;
            nfound++;
        }
    }
    return (seq_t)found[n - 1];
}
