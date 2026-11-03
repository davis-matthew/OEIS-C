/*
 * A159296  a(n) is the smaller number in the pair (L,m) which minimizes the
 *          primes of the form L^2 + m^2 under the constraint L + m = 2n + 1.
 * https://oeis.org/A159296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smaller number L of the pair (L, m) with L + m = 2n
 *            + 1 such that L^2 + m^2 is prime. Among all such pairs, the one
 *            for which this prime is minimal is chosen; the function returns
 *            L. (The algorithm simply searches the valid L in descending
 *            order from n down to 1.)
 * Range:     u8 n <= 11, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-03)
 * Helpers:   gcd, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/gcd.h"
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

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    idx_t sum = 2 * n + 1;           /* required L + m */
    seq_t one = seq_one();           /* constant 1 in seq_t */

    for (idx_t L = n; L >= 1; --L) {
        idx_t m  = sum - L;
        seq_t Lt = (seq_t)L;
        seq_t Mt = (seq_t)m;

        /* coprime check, required for prime sum of squares */
        if (gcd(Lt, Mt) != one)
            continue;

        seq_t p = Lt * Lt + Mt * Mt;
        if (is_prime_big(p))
            return Lt;               /* L is the desired smaller part */
    }

    /* In theory this point is never reached. */
    return (seq_t)0;
}
