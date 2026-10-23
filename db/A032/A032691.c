/*
 * A032691  Exactly 1 digit from {1,2,3,4,5,6,7,8,9} can precede a term to
 *          form a prime.
 * https://oeis.org/A032691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8096, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032691: a(n) = the n-th integer x such that exactly one
   prefix digit d∈{1,…,9} yields a prime when concatenated as d·10^k + x,
   where k is the number of decimal digits of x. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

static inline int seq_digit_len(seq_t v) {
    int l = 0;
    do {
        ++l;
        v /= (seq_t)10;
    } while (v != (seq_t)0);
    return l;
}

static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;   /* undefined offset */

    idx_t found = 0;
    seq_t x = (seq_t)1;

    while (1) {
        int len = seq_digit_len(x);
        int prime_count = 0;

        if (sizeof(seq_t) <= 8) {
            /* Small-sized seq_t: use 64‑bit intermediates. */
            uint64_t pow10 = 1;
            for (int i = 0; i < len; ++i) pow10 *= 10ULL;
            for (int d = 1; d <= 9; ++d) {
                uint64_t cand = pow10 * (uint64_t)d + (uint64_t)x;
                if (is_prime_uint64(cand)) {
                    ++prime_count;
                    if (prime_count > 1) break;
                }
            }
        } else {
            /* seq_t is at least 64 bits or arbitrary precision. */
            seq_t pow10 = (seq_t)1;
            for (int i = 0; i < len; ++i) pow10 *= (seq_t)10;
            for (int d = 1; d <= 9; ++d) {
                seq_t cand = pow10 * (seq_t)d + x;
                if (isPrimeGeneric(cand)) {
                    ++prime_count;
                    if (prime_count > 1) break;
                }
            }
        }

        if (prime_count == 1) {
            ++found;
            if (found == n) return x;
        }
        ++x;
    }
}
