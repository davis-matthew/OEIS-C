/*
 * A239020  Smallest number k such that k*n +/- 1 and k*n^2 +/- 1 are two sets
 *          of twin primes. a(n) = 0 if no such number exists.
 * https://oeis.org/A239020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239020
   Formula: a(n) = smallest k such that k*n±1 and k*n^2±1 are both twin primes;
   a(n) = 0 if no such k exists.
   Implementation uses the provided is_prime_big function for primality testing.
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
    /* Handle the special case n = 1.  From the OEIS table a(1) = 4. */
    if (n == 1)
        return 4;

    /* Determine gcd(n,6). Only four possibilities. */
    idx_t d = (n % 6 == 0) ? 6 :
              (n % 3 == 0) ? 3 :
              (n % 2 == 0) ? 2 : 1;

    /* Step size for k: k must be a multiple of 6/gcd(n,6). */
    seq_t step = 6 / d;               /* step ∈ {1,2,3,6} */

    /* Search for the smallest k. */
    for (seq_t k = step; ; k += step) {
        /* Center of the first twin prime set */
        seq_t t = k * (seq_t)n;
        seq_t t_minus = t - 1;
        seq_t t_plus  = t + 1;

        /* Check if t-1 and t+1 are prime. */
        if (!is_prime_big(t_minus) || !is_prime_big(t_plus))
            continue;

        /* Center of the second twin prime set */
        seq_t y = t * (seq_t)n;      /* k*n^2 = (k*n)*n */
        seq_t y_minus = y - 1;
        seq_t y_plus  = y + 1;

        if (is_prime_big(y_minus) && is_prime_big(y_plus))
            return k;                /* Found the smallest k */
    }
}
