/*
 * A178948  a(n) = smallest m>=0 such that n*(3^m)-1 is prime, or -1 if no
 *          such prime exists.
 * https://oeis.org/A178948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find smallest m ≥ 0 such that n*3^m – 1 is prime;
   return –1 if no such prime exists for the given n. */
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
    /* For odd n ≥ 5 the expression n·3^m – 1 is even and > 2,
       hence never prime. */
    if ((n & 1) && n >= 5)
        return (seq_t)-1;

    /* Maximum representable positive value of type seq_t. */
    const __int128 max_val = (((__int128)1) << (sizeof(seq_t) * 8 - 1)) - 1;

    seq_t pow3 = 1;          /* 3^m */
    idx_t  m   = 0;

    for (;; ++m)
    {
        /* Compute n * 3^m in a wider type to detect overflow. */
        __int128 prod = (__int128)n * (__int128)pow3;
        if (prod > max_val)          /* would exceed seq_t range */
            break;

        seq_t val = (seq_t)(prod - 1);
        if (is_prime_big(val))
            return (seq_t)m;

        /* Prepare next power of 3, checking that the product stays
           within the range of seq_t. */
        if ((__int128)pow3 > max_val / 3)
            break;                  /* pow3 * 3 would overflow seq_t */
        pow3 *= 3;
    }

    return (seq_t)-1;
}
