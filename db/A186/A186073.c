/*
 * A186073  Irregular triangle, read by rows, of primes with suffix n and
 *          digits "7" prepended, otherwise 0.
 * https://oeis.org/A186073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) is defined by iterating over rows of an irregular triangle.
 *            For a row with suffix r (r = 1,2,3,…) the terms are the primes
 *            of the form 7…7r – that is, one or more leading 7‑digits
 *            followed by r, stopping as soon as the first composite is
 *            encountered. The flattened sequence is obtained by con...
 * Range:     u8 n <= 1, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    idx_t idx = 1;          /* current linear index */
    for (idx_t r = 1; ; ++r) {
        seq_t r_seq = (seq_t)r;
        int d = num_digits(r_seq);
        seq_t pow10_d = pow10((idx_t)d);
        seq_t leading = 0;          /* leading 7‑digits as a number */
        bool found = false;        /* whether this row produced any prime */

        for (;;) {
            /* prepend one more 7 */
            leading = leading * 10 + 7;
            seq_t val = leading * pow10_d + r_seq;

            if (is_prime_big(val)) {
                if (idx == n) return val;
                ++idx;
                found = true;
            } else {
                /* first composite ends the row */
                break;
            }
        }

        if (!found) {              /* row had no primes -> output 0 */
            if (idx == n) return 0;
            ++idx;
        }

        if (idx > n) break;        /* safety: we have already returned */
    }

    /* should never be reached for valid n */
    return 0;
}
