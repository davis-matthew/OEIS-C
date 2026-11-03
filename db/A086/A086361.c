/*
 * A086361  a(n)=A085956(3n).
 * https://oeis.org/A086361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the smallest prime p such that
 *   p is prime,
 *   (p-1)/(6n) is prime,
 *   and 6n*p + 1 is prime.
 * This is equivalent to a(n) = A085956(3n).
 *
 * Implementation: iterate over primes q = (p-1)/(6n), compute
 *   p = 6n * q + 1,
 *   test p and 6n*p+1 for primality.
 * The first q producing both primes yields a(n).
 * The routine uses is_prime_big for arbitrary‑precision
 * primality checking (works for 64‑bit and larger builds).
 */
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n) {
    /* Ensure n >= 1 (offset 1) */
    if (n == 0) return 0;

    /* tn = 6 * n  (type seq_t to avoid overflow) */
    seq_t tn = (seq_t)n * 6;

    /* Iterate over prime q */
    for (uint64_t q = 2; ; ++q) {
        if (!is_prime_big((seq_t)q))
            continue;               /* q must be prime */

        /* Compute p = tn * q + 1 */
        seq_t p = tn * (seq_t)q + 1;
        if (!is_prime_big(p))
            continue;               /* p must be prime */

        /* Check that 6n * p + 1 is prime */
        seq_t r = tn * p + 1;
        if (is_prime_big(r))
            return p;               /* Found the required prime */
    }
}
