/*
 * A174414  a(n) is the smallest natural number k such that the concatenation
 *          (n+k)||k is a prime number.
 * https://oeis.org/A174414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that the concatenation (n + k)||k is
 *            prime
 * Range:     u8 n <= 10, u16 n <= 1242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9665 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
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
    /* Converting index to seq_t once for arithmetic with seq_t values */
    const seq_t nseq = (seq_t)n;

    /* Search for the smallest k satisfying the condition */
    for (idx_t kk = 1; ; ++kk)
    {
        /* Only odd last digits can yield a prime (except 2) */
        const int last = kk % 10;
        if (!(last == 1 || last == 3 || last == 7 || last == 9))
            continue;

        /* k as seq_t for arithmetic */
        const seq_t k = (seq_t)kk;

        /* m = n + k */
        const seq_t m = nseq + k;

        /* If gcd(n, 10^d + 1) ≠ 1 we can skip this k (where d = digits(k)) */
        const int d = num_digits(k);
        const seq_t pow10d = pow10((idx_t)d);
        const seq_t mod = pow10d + (seq_t)1;
        if (gcd(nseq, mod) != (seq_t)1)
            continue;

        /* Concatenate (n + k) and k.  The helper expects idx_t arguments. */
        const seq_t candidate = concat_parts((idx_t)(n + kk), (idx_t)kk);

        /* Check primality */
        if (is_prime_big(candidate))
            return k;        /* k is already a seq_t, return it as the answer */
    }
}
