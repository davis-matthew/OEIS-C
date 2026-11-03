/*
 * A082442  Palindromic primes with middle digit 6.
 * https://oeis.org/A082442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 400, u64 n <= 400, u128 n <= 400,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 386 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) is the n‑th palindromic prime whose length is odd
   and whose middle digit is 6.  The function generates palindromes
   incrementally, tests for primality using is_prime_big, and returns
   the requested term.  The algorithm runs in O(m) time where m is the
   number of candidates examined. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_digits_u64.h"
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
    idx_t count = 0;           /* number of valid primes found */

    /* len_left = number of digits in the left half of the odd‑length
        palindrome (excluding the middle digit 6). */
    for (idx_t len_left = 1;; ++len_left) {
        /* smallest and largest values for the left half; no leading zeros */
        idx_t start = (len_left == 1) ? 1 : (idx_t)pow10(len_left - 1);
        idx_t end   = (idx_t)pow10(len_left) - 1;

        /* 10^len_left  and  10^(len_left+1) as seq_t for assembling
           the full palindrome.  Reuse these values for every left half
           of the current length. */
        seq_t pow_len_left      = pow10(len_left);      /* 10^len_left      */
        seq_t pow_len_left_plus = pow10(len_left + 1);  /* 10^(len_left+1) */

        for (idx_t L = start; L <= end; ++L) {
            /* reverse the left half to form the right half */
            seq_t rev = reverse_digits_u64(L);

            /* build the full palindrome:
                 [left] 6 [reverse(left)]  */
            seq_t val = (seq_t)L * pow_len_left_plus
                       + (seq_t)6 * pow_len_left
                       + rev;

            if (is_prime_big(val)) {
                ++count;
                if (count == n) {
                    return val;
                }
            }
        }
    }
}
