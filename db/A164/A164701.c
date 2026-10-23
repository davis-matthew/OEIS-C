/*
 * A164701  a(n) = the smallest positive integer such that neither a(n) nor
 *          a(n)+1 are coprime to A024619(n), where A024619 is the sequence of
 *          positive integers that are not powers of primes.
 * https://oeis.org/A164701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by searching for the smallest m such that
   gcd(m, s) > 1 and gcd(m+1, s) > 1, where s =
   the nth positive integer that is NOT a power of a prime
   (A024619). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* test whether a 64‑bit integer is a power of a prime */
static inline bool is_prime_power_uint64(uint64_t x)
{
    if (x < 2) return false;          /* 1 is not a prime power */
    uint64_t d, spf = 0;
    for (d = 2; d * d <= x; ++d) {
        if (x % d == 0) {
            spf = d;
            break;
        }
    }
    if (spf == 0) return true;        /* x is prime */
    uint64_t tmp = x;
    while (tmp % spf == 0) tmp /= spf;
    return tmp == 1;
}

seq_t a(idx_t n)
{
    /* determine the nth non‑prime‑power number (A024619(n)) */
    uint64_t candidate = 2;
    idx_t count = 0;
    uint64_t s = 0;
    while (true) {
        if (!is_prime_power_uint64(candidate)) {
            ++count;
            if (count == n) {
                s = candidate;
                break;
            }
        }
        ++candidate;
    }

    /* find the smallest m such that both m and m+1 share a factor with s */
    seq_t m = 2;
    seq_t ss = (seq_t)s;
    while (true) {
        if (gcd(m, ss) > 1 && gcd(m + 1, ss) > 1) {
            return m;
        }
        ++m;
    }
}
