/*
 * A120609  Primes among the absolute value of numbers of the form f(x)= x^2 +
 *          x - 1354363.
 * https://oeis.org/A120609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120609: the n‑th prime among the absolute values of
   f(x) = x^2 + x - 1354363 (x >= 1). */

#include <stdbool.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    if (n == 0) abort();                /* undefined for 0 based on specification */

    idx_t count = 0;
    const seq_t offset = (seq_t)1354363; /* constant part of the quadratic */

    for (idx_t x = 1; ; ++x) {
        /* Compute x*(x+1) using the seq_t type directly. */
        seq_t prod = (seq_t)x * ((seq_t)x + (seq_t)1);

        /* Take the absolute difference from the offset. */
        seq_t val;
        if (prod >= offset)
            val = prod - offset;
        else
            val = offset - prod;

        /* Zero cannot occur for integer x here, but test just in case. */
        if (val == 0) continue;

        /* Check primality. */
        if (is_prime_big(val)) {
            ++count;
            if (count == n) return val;
        }
    }
}
