/*
 * A185145  Smallest average of twin prime pairs s such that n*s is also
 *          average of twin prime pairs.
 * https://oeis.org/A185145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest average s of twin prime pairs such that *
 *            n*s is also an average of twin prime pairs. * (Implementation:
 *            search over odd s, test primality conditions.)
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 85 terms through n = 4556 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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
    /* Offset: a(1) corresponds to the first average (4). */
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    unsigned long long s = 4;   /* first candidate average (3,5) => 4 */
    for (;;) {
        /* Check if s is an average of a twin prime pair: (s-1, s+1) primes */
        if (is_prime_big((seq_t)(s - 1)) && is_prime_big((seq_t)(s + 1))) {
            seq_t ns = (seq_t)n * (seq_t)s;
            /* Verify that n*s is also an average of a twin prime pair */
            if (is_prime_big(ns - (seq_t)1) && is_prime_big(ns + (seq_t)1))
                return (seq_t)s;   /* found minimal s */
        }
        s += 2;   /* advance to next odd number */
    }
}
