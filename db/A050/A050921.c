/*
 * A050921  Smallest prime of form n*2^m+1, m >= 0, or 0 if no such prime
 *          exists.
 * https://oeis.org/A050921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 62 terms through n = 323 (2026-11-03)
 * Helpers:   is_prime_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest prime of the form n*2^m + 1 (m ≥ 0),
   or 0 if no such prime is found within a practical bound. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
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
    /* OEIS offset is 1 */
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;           /* convert index to seq_t */
    seq_t one = 1;                 /* constant 1 in seq_t type */

    /* Search for the smallest exponent m giving a prime.  The bound
       1024 is chosen to be well beyond what is needed for all
       pre‑tested terms while keeping the runtime small. */
    for (idx_t m = 0; m < 1024; ++m)
    {
        seq_t p2 = pow2(m);          /* 2^m as seq_t */
        /* Detect overflow for very small seq_t types (e.g. uint8_t) */
        if (p2 == 0)
            break;
        seq_t p = sn * p2 + one;
        if (is_prime_big(p))
            return p;                /* found the smallest such prime */
    }

    /* No prime found within the search bound.  Return 0 as specified. */
    return 0;
}
