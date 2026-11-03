/*
 * A070331  Numbers in which suffixing or inserting a 0 anywhere yields only
 *          composite numbers.
 * https://oeis.org/A070331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   numbers n such that inserting a 0 between digits or at the end
 *            yields only composite numbers (OEIS A070331)
 * Range:     u8 n <= 28, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Helper: check whether a decimal number satisfies the zero‑insertion
   composite property. */
static bool ok(seq_t x) {
    int d = num_digits(x);               /* number of decimal digits */
    seq_t pow10[64];                    /* 10^k, long enough for all d */
    pow10[0] = 1;
    for (int i = 1; i <= d; ++i) pow10[i] = pow10[i-1] * 10;

    for (int off = 0; off < d; ++off) {
        seq_t left  = x / pow10[d - off - 1];
        seq_t right = x % pow10[d - off - 1];
        seq_t ins   = left * pow10[d - off] + right;  /* insert 0 */
        if (is_prime_big(ins)) return false;          /* a prime appears */
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t found = 0;          /* how many qualifying numbers found so far */
    idx_t cand  = 1;          /* current candidate to test */

    while (found < n) {
        if (ok((seq_t)cand)) ++found;  /* qualifies → count it */
        ++cand;                        /* try next integer */
    }
    return (seq_t)(cand - 1);           /* last found number */
}
