/*
 * A092908  Primes in A051022.
 * https://oeis.org/A092908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search -- a(n) is the n-th prime among A051022(k), k >= 1, where
 *            A051022(k) is k with a 0 inserted between each pair of adjacent
 *            decimal digits
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 898 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t A092908_interleave(idx_t n)
{
    seq_t result = 0;
    seq_t p = 1;
    while (n > 0) {
        result += (seq_t)(n % 10) * p;
        p *= 100;
        n /= 10;
    }
    return result;
}

seq_t a(idx_t n)
{
    static idx_t last_n = 0, last_k = 0;
    static seq_t last_v = 0;
    idx_t count, k;
    if (last_n != 0 && n == last_n) return last_v;
    if (last_n != 0 && n > last_n) { count = last_n; k = last_k; }
    else { count = 0; k = 0; }
    seq_t v = 0;
    while (count < n) {
        k++;
        idx_t d = k % 10;
        if (k >= 10 && (d % 2 == 0 || d == 5)) continue;
        v = A092908_interleave(k);
        if (is_prime_seq(v)) count++;
    }
    last_n = n; last_k = k; last_v = v;
    return v;
}
