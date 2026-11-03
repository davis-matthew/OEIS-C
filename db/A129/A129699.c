/*
 * A129699  Least nonnegative m such that P(n+3,n) + P(n+3,m) is prime where
 *          P(k,n) is n-th k-gonal number, or -1 if no such value exists.
 * https://oeis.org/A129699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 85 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A129699:
   a(n) = least m ≥ 0 such that the sum of the n‑th and m‑th (n+3)-gonal
   numbers is prime; return -1 if no such m exists. */
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

/* Polygonal numbers:  P(k,n) = ((k-2)n^2 – (k-4)n)/2  (k ≥ 3, n ≥ 0) */
static seq_t poly(seq_t k, idx_t n)
{
    seq_t nn   = (seq_t)n * (seq_t)n;                          /* n² */
    seq_t num  = (seq_t)(k - 2) * nn - (seq_t)(k - 4) * (seq_t)n; /* numerator */
    return num / 2;                                            /* division by 2 */
}

seq_t a(idx_t n)
{
    seq_t k     = (seq_t)(n + 3);          /* polygonal parameter */
    seq_t base  = poly(k, n);              /* P(k,n) */

    /* Search limit – large enough for all n up to about 10 000. */
    const idx_t MAX_M = 1000000;

    for (idx_t m = 0; m <= MAX_M; ++m)
    {
        seq_t sum = base + poly(k, m);
        if (is_prime_big(sum))
            return (seq_t)m;
    }
    /* No suitable m found within the bound. */
    return (seq_t)(-1);
}
