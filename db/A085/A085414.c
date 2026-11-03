/*
 * A085414  Primes that are a concatenation of a prime and its first digit.
 * https://oeis.org/A085414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = nth prime p such that there exists a prime q with
 *                p = concat(q, first_digit(q)).
 * Computation: enumerate prime q, construct p, and test p for primality.
 *              Uses the helper functions is_prime (for q) and
 *              is_prime_big (for p).  Only a small helper computes the
 *              most significant decimal digit of q.
 *
 *  The OEIS offset is 1: a(1) is 131.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

/* Return the most significant decimal digit of a 64‑bit integer. */
static uint64_t first_digit_uint64(uint64_t x)
{
    while (x >= 10)
        x /= 10;
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n < 1 */

    idx_t found = 0;                    /* count of valid terms found */
    idx_t q = 2;                        /* candidate prime q */

    for (;; ++q)
    {
        if (!is_prime(q))
            continue;                   /* skip non‑prime q */

        uint64_t d = first_digit_uint64(q);   /* first (most significant) digit */

        /* p = concat(q, d) = q*10 + d */
        seq_t p = (seq_t)q * (seq_t)10 + (seq_t)d;

        if (is_prime_big(p))
        {
            ++found;
            if (found == n)
                return p;
        }
    }
}
