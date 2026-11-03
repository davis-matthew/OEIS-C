/*
 * A222724  Palindromic nonprime numbers starting with a digit 1.
 * https://oeis.org/A222724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 969 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth palindromic nonprime number that starts with the digit 1.
   The index is 1‑based (a(1) = 1). */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_digits.h"
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
    /* Count of palindromes found so far */
    idx_t count = 0;

    /* Generate palindromes by increasing length */
    for (idx_t len = 1; ; ++len)
    {
        /* half = ceil(len/2) – number of digits in the left half (including the middle digit if odd) */
        idx_t half = (len + 1) / 2;

        seq_t prefix_start = pow10(half - 1);   /* first digit fixed to 1 */
        seq_t prefix_end   = prefix_start * 2 - 1;   /* only prefixes that begin with 1 */

        seq_t pow10_half   = pow10(half);
        seq_t pow10_halfm1 = pow10(half - 1);

        if (len & 1) /* odd length */
        {
            for (seq_t prefix = prefix_start; prefix <= prefix_end; ++prefix)
            {
                seq_t pal = prefix * pow10_halfm1 + reverse_digits(prefix / 10);
                if (!is_prime_big(pal))
                {
                    ++count;
                    if (count == n) return pal;
                }
            }
        }
        else /* even length */
        {
            for (seq_t prefix = prefix_start; prefix <= prefix_end; ++prefix)
            {
                seq_t pal = prefix * pow10_half + reverse_digits(prefix);
                if (!is_prime_big(pal))
                {
                    ++count;
                    if (count == n) return pal;
                }
            }
        }
    }

    /* Should never reach here for valid n */
    return 0;
}
