/*
 * A229107  Palindromic prime time display in hours, minutes, seconds on a
 *          six-digit 24-hour digital clock.
 * https://oeis.org/A229107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: full, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   is_palindrome_base, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A229107 by brute‑force enumeration of all times 00:00:00‑23:59:59,
   keeping only those whose numeric representation (leading zeros omitted)
   is a palindrome in base 10 and a prime.  The list contains 69 terms. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t cache[70];
static idx_t cached_len = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        cached_len = 0;

        for (idx_t h = 0; h < 24; ++h) {
            for (idx_t m = 0; m < 60; ++m) {
                for (idx_t s = 0; s < 60; ++s) {
                    /* numeric value after removing leading zeros: hhmmss */
                    idx_t val = h * 10000 + m * 100 + s;
                    seq_t val_seq = (seq_t)val;
                    if (is_palindrome_base(val_seq, 10) && is_prime_big(val_seq)) {
                        if (cached_len < 70) {
                            cache[cached_len++] = val_seq;
                        }
                    }
                }
            }
        }
    }

    if (n >= 1 && n <= cached_len) {
        return cache[n - 1];
    }
    return (seq_t)0; /* out‑of‑range request */
}
