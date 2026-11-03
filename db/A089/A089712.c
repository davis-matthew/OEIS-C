/*
 * A089712  Smallest prime as a concatenation k and k-n.
 * https://oeis.org/A089712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A089712: a(n) = smallest prime that can be written as
 *   concatenation of an integer k and k-(n-1),
 * with k ≥ max(1, n-1).  The sequence is indexed from 1 (offset 1).  */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits.h"
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

/* Compute 10^e in seq_t. */
static seq_t pow10_seq(seq_t e) {
    seq_t r = 1;
    while (e-- > 0) {
        r *= 10;
    }
    return r;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Convert the 1‑based index to the underlying n₀ = n‑1 used in the
       definition.  */
    if (n == 0) {
        return 0;          /* undefined for n < 1 */
    }
    idx_t n0 = n - 1;      /* n₀ = n-1 (may be 0 for the first term) */

    /* Start searching for k.  k must be at least 1, and k-n₀ must be
       non‑negative, so k ≥ n₀.  The smallest allowable k is:
          k = 1, when n₀ == 0
          k = n₀, when n₀ > 0
       For n₀ > 0, the value k=n₀ produces a trailing part of 0, which
       is handled specially below.  */
    idx_t k = (n0 == 0) ? (idx_t)1 : n0;

    for (;;) {
        idx_t r = k - n0;              /* r = k - (n-1) */
        seq_t p;

        if (r == 0) {                  /* r == 0 → need one more digit (the digit '0') */
            p = (seq_t)k * (seq_t)10;   /* concatenation of k and 0 */
        } else {
            seq_t mult = pow10_seq((seq_t)num_digits((seq_t)r));
            p = (seq_t)k * mult + (seq_t)r;
        }

        if (is_prime_big(p)) {
            return p;                  /* first (hence smallest) prime found */
        }
        ++k;                           /* try next k */
    }
}
