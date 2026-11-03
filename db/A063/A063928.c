/*
 * A063928  Largest nonprime proper divisor of n (with a(1)=1).
 * https://oeis.org/A063928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest nonprime proper divisor of n (with a(1)=1)
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 105 terms through n = 979 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

/* Largest nonprime proper divisor of n */
seq_t a(idx_t n)
{
    /* a(1) is defined to be 1; for n=1 we return 1 directly. */
    if (n <= 2) {
        /* For n = 1 or 2: proper divisors are {1};
           1 is not prime, so return 1. */
        return 1;
    }

    /* Find the smallest prime factor (spf) of n.  If none are found,
       n is prime. */
    idx_t spf = 0;
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            spf = d;
            break;
        }
    }
    if (spf == 0) {
        /* n is prime, only proper divisor is 1. */
        return 1;
    }

    /* Candidate largest nonprime divisor is n / spf. */
    idx_t q = n / spf;
    if (q > 1 && !is_prime_big((seq_t)q)) {
        /* If q is composite, it is the largest nonprime proper divisor. */
        return (seq_t)q;
    }

    /* If q is prime (or q==1), the only nonprime proper divisor is 1. */
    return 1;
}
