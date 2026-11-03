/*
 * A157868  Palindromic primes with prime digital roots.
 * https://oeis.org/A157868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth palindromic prime with prime digital root
 * Range:     u8 n <= 9, u16 n <= 38, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, num_digits_idx, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A157868 */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits_idx.h"
#include "../utils/reverse_digits.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static inline bool has_prime_droot(seq_t n) {
    seq_t r = n % (seq_t)9u;
    if (r == 0u) r = 9u;
    unsigned int dr = (unsigned int)r;
    return dr == 2u || dr == 3u || dr == 5u || dr == 7u;
}

static inline seq_t make_pal(seq_t half, bool odd) {
    /* number of decimal digits of half */
    idx_t len = num_digits_idx((idx_t)half);
    seq_t rev, pow;
    if (odd) {
        seq_t half_div10 = half / 10u;
        rev = reverse_digits(half_div10);
        pow = pow10(len - 1);
    } else {
        rev = reverse_digits(half);
        pow = pow10(len);
    }
    return half * pow + rev;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1    */
    idx_t count = 0;

    for (idx_t len = 1; ; ++len) {
        bool odd = (len & 1);
        idx_t half_len = (len + 1) / 2;

        /* smallest half: 10^(half_len-1), largest: 10^half_len - 1 */
        idx_t start = 1;
        for (idx_t i = 1; i < half_len; ++i) start *= 10;
        idx_t end   = start * 10 - 1;

        for (idx_t half = start; half <= end; ++half) {
            seq_t pal = make_pal((seq_t)half, odd);
            if (isPrimeGeneric(pal) && has_prime_droot(pal)) {
                ++count;
                if (count == n) return pal;
            }
        }
    }
}
