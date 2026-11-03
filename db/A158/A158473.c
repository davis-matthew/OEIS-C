/*
 * A158473  Primes whose digit sum contains one or more digits of the same
 *          prime.
 * https://oeis.org/A158473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 4391, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 7963 (2026-11-03)
 * Helpers:   digit_sum, get_digits, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Locate primes in increasing order and collect those
   whose digit sum shares at least one digit with the prime itself.
   The sequence is indexed starting at 1 (a(1)=2). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/get_digits.h"
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

/* Maximum number of terms we will cache.  Ten thousand is enough for
   typical use but a safety margin is added. */
#define CACHE_CAP 20000

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;        /* number of terms cached so far   */
static uint64_t last_candidate = 2; /* next integer to test for primality */

/* Return the n‑th term (1‑based).  n ≥ 1 by definition. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* defensive, though the OEIS offset is 1 */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n && cache_len < CACHE_CAP) {
        if (is_prime_big((seq_t)last_candidate)) {
            /* Compute the digit sum of the prime. */
            seq_t sum = digit_sum((seq_t)last_candidate);

            /* Build bit masks for the digits of the prime and its sum. */
            uint8_t digits[64];         /* will hold the digits of a number */
            size_t len;
            uint16_t mask_prime = 0, mask_sum = 0;

            get_digits((seq_t)last_candidate, digits, &len);
            for (size_t i = 0; i < len; ++i)
                mask_prime |= 1 << (digits[i] & 0xF);

            get_digits(sum, digits, &len);
            for (size_t i = 0; i < len; ++i)
                mask_sum |= 1 << (digits[i] & 0xF);

            /* Record the prime if the intersection is non‑empty. */
            if (mask_prime & mask_sum) {
                cache[cache_len++] = (seq_t)last_candidate;
            }
        }
        ++last_candidate;
    }

    /* If the requested index exceeds the pre‑allocated capacity, return
       0 as a sentinel.  In practice this should not happen for the
       intended range. */
    if (n > cache_len)
        return 0;

    return cache[n - 1];
}
