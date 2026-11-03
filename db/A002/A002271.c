/*
 * A002271  Numbers m such that all odd numbers k, 1 < k < m, relatively prime
 *          to m are primes.
 * https://oeis.org/A002271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: full, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: m is a term if every odd k with 1 < k < m and gcd(k,m) = 1 is prime (finite sequence, last term 105) */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"

static bool a2271_ok(uint64_t m) {
    for (uint64_t k = 3; k < m; k += 2)
        if (gcd_u64(k, m) == 1 && !is_prime_u64(k))
            return false;
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    /* the sequence is finite; bound the search so out-of-range n terminates */
    for (uint64_t m = 2; m <= 10000; m++) {
        if (a2271_ok(m)) {
            count++;
            if (count == n) return (seq_t)m;
        }
    }
    return (seq_t)0;
}
