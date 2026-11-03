/*
 * A032721  Numbers k such that k prefixed by '4' and followed by '7' is
 *          prime.
 * https://oeis.org/A032721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 289, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 984 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers k such that the decimal number formed by prefixing a '4'
 *  and appending a '7' to k (i.e., 4·10^(d+1)+k·10+7, where d is the
 *  number of decimal digits of k) is prime.
 *  The function returns the n‑th such k with OEIS offset 1.
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
    /*  n < 1 is undefined for this sequence (offset = 1) */
    if (n < 1) return 0;

    idx_t found = 0;      /* how many suitable k's we have found */
    idx_t kidx  = 0;      /* candidate integer, treated as idx_t for safety */
    seq_t result = 0;     /* value to return */

    while (found < n) {
        ++kidx;
        seq_t k = (seq_t)kidx;

        /* number of decimal digits of k */
        int d = num_digits(k);

        /* 4·10^(d+1) */
        idx_t exp = (idx_t)(d + 1);
        seq_t pow10_d = pow10(exp);

        /* construct M = 4·10^(d+1) + k·10 + 7 */
        seq_t M = ((seq_t)4) * pow10_d + k * 10 + 7;

        if (is_prime_big(M)) {
            ++found;
            if (found == n) {
                result = k;
                break;
            }
        }
    }

    return result;
}
