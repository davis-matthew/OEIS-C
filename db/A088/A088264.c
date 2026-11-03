/*
 * A088264  Smallest number k > 0 such that prefixing k to the n-th quadruple
 *          in the set {(1,3,7,9), (11,13,17,19), (21,23,27,29), ...} yields
 *          all primes.
 * https://oeis.org/A088264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9074 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088264
 *  a(n) = smallest k > 0 such that the numbers
 *          k*10^d + (10·(n−1)+1, 10·(n−1)+3, 10·(n−1)+7, 10·(n−1)+9)
 *  are all prime, where d = digits(10·(n−1)+1).
 *
 *  The implementation searches k sequentially, testing the four
 *  candidates for primality.  For values that fit in 64 bits the
 *  deterministic Miller‑Rabin test is used; for larger values the
 *  arbitrary‑precision test provided by the utilities is invoked.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* number of decimal digits of an unsigned 64‑bit integer (x > 0) */
static inline int digits_uint64(uint64_t x)
{
    int d = 1;
    while (x >= 10ULL) { x /= 10ULL; ++d; }
    return d;
}

/* primality test for seq_t values: use 64‑bit test if possible,
 * otherwise call the arbitrary‑precision routine. */
static inline bool is_prime_any(seq_t n)
{
    if (n <= (seq_t)UINT64_MAX)
        return is_prime_u64((uint64_t)n);
    return is_prime_big(n);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n=0 */

    /* base = 10·(n−1) as uint64_t */
    uint64_t base = 10ULL * ((uint64_t)n - 1ULL);

    /* number of digits of the quadruple members */
    int d = digits_uint64(base + 1ULL);

    /* factor = 10^d as seq_t */
    seq_t factor = pow10((idx_t)d);

    /* offsets of the four members of the quadruple */
    const int ofs[4] = {1, 3, 7, 9};

    seq_t k = 1;                          /* candidate k > 0 */
    for (;;) {
        bool all_prime = true;
        for (int i = 0; i < 4; ++i) {
            seq_t val = factor * k + (seq_t)(base + (uint64_t)ofs[i]);
            if (!is_prime_any(val)) {
                all_prime = false;
                break;
            }
        }
        if (all_prime) return k;
        ++k;
    }
}
