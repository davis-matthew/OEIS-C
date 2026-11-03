/*
 * A038370  Smallest prime with "n^2" as central digit(s).
 * https://oeis.org/A038370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest prime whose decimal representation has
   'n^2' as the centered block of digits.  For a given n
   we search over increasing widths k of the equal
   number of digits on both sides, generating all
   numbers left * 10^(m+k) + n^2 * 10^k + right,
   where m is the number of decimal digits of n^2,
   left ranges over 10^(k-1)..10^k-1 and
   right ranges over 0..10^k-1.  The first prime found
   for a given k is returned. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"
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

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;                 /* n^2          */
    idx_t   m  = (sq == 0) ? 1 : num_digits_idx((idx_t)sq); /* digits of n^2 */

    /* search over increasing side width k */
    for (idx_t k = 1; ; ++k)
    {
        seq_t pow10_k   = pow10(k);           /* 10^k          */
        seq_t pow10_k1  = pow10(k-1);         /* 10^(k-1)      */
        seq_t mult      = pow10(m + k);       /* 10^(m+k)      */
        seq_t mid       = sq * pow10_k;       /* n^2 * 10^k    */

        idx_t left_start = (idx_t)pow10_k1;
        idx_t left_end   = (idx_t)(pow10_k - 1);

        for (idx_t left = left_start; left <= left_end; ++left)
        {
            seq_t base = (seq_t)left * mult + mid;
            for (idx_t right = 0; right < (idx_t)pow10_k; ++right)
            {
                seq_t candidate = base + (seq_t)right;
                if (is_prime_big(candidate))
                    return candidate;
            }
        }
    }
}
