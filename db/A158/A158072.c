/*
 * A158072  Primes p such that the sum of digits of p - the first digit of p
 *          is prime.
 * https://oeis.org/A158072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   digit_sum, is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p such that the sum of the digits of p
   minus its first digit is a prime.  No simple closed form is known,
   so primes are generated on demand and the qualifying ones are cached. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"
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

/* Cache of qualifying primes */
static seq_t *qual_primes = NULL;
static size_t qual_len = 0, qual_cap = 0;
static bool initialized = false;
static idx_t next_candidate = 2;

/* Ensure at least 'n' qualifying primes are available in qual_primes */
static void ensure_qualifiers(idx_t n)
{
    if (!initialized) {
        initialized = true;
        qual_cap = 1024;
        qual_primes = mem_new(seq_t, qual_cap);
        qual_len = 0;
        next_candidate = 2;
    }

    while (qual_len < (size_t)n) {
        /* Find next prime candidate */
        while (!is_prime(next_candidate)) {
            next_candidate += (next_candidate == 2 ? 1 : 2);
        }
        idx_t p = next_candidate;

        /* Advance candidate for the next search */
        next_candidate += (p == 2 ? 1 : 2);

        /* Sum of digits of p */
        seq_t sum = digit_sum((seq_t)p);

        /* First (most significant) digit of p */
        idx_t temp = p;
        while (temp >= 10)
            temp /= 10;
        seq_t first_digit = (seq_t)temp;

        /* Difference sum - first_digit */
        seq_t diff = sum - first_digit;

        /* Check whether diff is > 1 and prime */
        if (diff > (seq_t)1 && is_prime_big(diff)) {
            /* Store qualifying prime */
            if (qual_len == qual_cap) {
                qual_cap <<= 1;
                qual_primes = mem_resize(seq_t, qual_primes, qual_cap);
            }
            qual_primes[qual_len++] = (seq_t)p;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_qualifiers(n);
    return qual_primes[n - 1];  /* offset 1 */
}
