/*
 * A088324  Smallest prime formed by a substring of the decimal digits of 1/n
 *          beginning with the first (nonzero) digit after the decimal point,
 *          or 0 if no such prime exists.
 * https://oeis.org/A088324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime that starts at the first non‑zero decimal
 *            digit of 1/n; returns 0 if no such prime exists
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-03)
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

seq_t a(idx_t n)
{
    if (n <= 1)                /* 1/1 has no fractional part */
        return 0;

    /* Current remainder of 1/n when extracting digits */
    uint64_t rem = 1 % (uint64_t)n;   /* 1 for all n>1 */
    bool started = false;           /* first non‑zero digit seen? */
    seq_t cur = 0;                  /* accumulated number */

    /* Upper bound on the number of digits examined.
       For a terminating decimal rem becomes 0 early.
       For a repeating decimal the period is at most n‑1,
       so 2*n iterations safely cover all possible prefixes.   */
    uint64_t max_digits = (n == 1) ? 0 : 2 * n;

    for (uint64_t i = 0; i < max_digits && rem != 0; ++i) {
        /* Generate next decimal digit of 1/n */
        uint64_t d = (rem * 10) / (uint64_t)n;
        rem     = (rem * 10) % (uint64_t)n;

        if (!started) {
            if (d == 0)         /* skip leading zeros */
                continue;
            started = true;     /* first non‑zero digit reached */
        }

        /* Append digit to the current number; stop where that wraps around
           in seq_t (the wrapped value was tested for primality) */
        seq_t next = cur * 10 + (seq_t)d;
        if ((next - (seq_t)d) / 10 != cur || next < cur) break;
        cur = next;

        /* Test for primality */
        if (is_prime_big(cur))
            return cur;
    }

    /* No prime found within the examined digits */
    return 0;
}
