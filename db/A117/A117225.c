/*
 * A117225  Primes of the form f(n) = 9*n^4 - 444*n^3 + 8059*n^2 - 63714*n +
 *          185371 listed by increasing value of n >= 0.
 * https://oeis.org/A117225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = f(k) where f(k) = 9*k^4 - 444*k^3 + 8059*k^2 - 63714*k + 185371
 * and k is the smallest non‑negative integer such that exactly n values of f(k)
 * have been found to be prime.  
 * Thus a(1)=f(0)=185371, a(2)=f(1)=129281, ...
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

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – behaviour for n=0 is undefined */
        return 0;

    idx_t found = 0;
    idx_t k = 0;   /* polynomial variable, starts at 0 */

    for (;;) {
        seq_t m  = (seq_t)k;
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        seq_t m4 = m3 * m;

        seq_t val =   9 * m4
                   - 444 * m3
                   + 8059 * m2
                   - 63714 * m
                   + 185371;

        if (is_prime_big(val)) {
            ++found;
            if (found == n)
                return val;
        }
        ++k;
    }
}
