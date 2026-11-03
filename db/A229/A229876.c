/*
 * A229876  Palindromic prime numbers == 2 (mod 9).
 * https://oeis.org/A229876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 994, u64 n <= 994,
 *            u128 n <= 994, big any n
 * Validated: standard, pass, 51 terms through n = 990 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A229876:
 *  Palindromic prime numbers ≡ 2 (mod 9).
 *  The function enumerates palindromic numbers by length,
 *  tests the congruence and primality using the supplied
 *  utilities is_prime_big and pow10.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/reverse_digits.h"
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
    /* n ≥ 1 by definition of the sequence */
    idx_t found = 0;

    for (idx_t len = 1; ; ++len) {
        /* Even-length palindromes (except 2‑digit 11) are divisible by 11 */
        if ((len % 2 == 0) && len > 2)
            continue;

        idx_t half = (len + 1) / 2;           /* digits in the left half */

        seq_t start   = pow10((idx_t)(half - 1));        /* smallest left half */
        seq_t end     = pow10((idx_t)half) - (seq_t)1;   /* largest left half */

        for (seq_t left = start; left <= end; ++left) {
            seq_t rev = (len % 2 == 1)
                        ? reverse_digits(left / (seq_t)10)
                        : reverse_digits(left);

            seq_t pow_val = pow10((idx_t)(len - half));
            seq_t pal = left * pow_val + rev;

            if (pal <= (seq_t)1)
                continue;

            /* Check congruence to 2 modulo 9. */
            if (pal % (seq_t)9 != (seq_t)2)
                continue;

            if (is_prime_big(pal)) {
                ++found;
                if (found == n)
                    return pal;
            }
        }
    }
}
