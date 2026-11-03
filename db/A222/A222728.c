/*
 * A222728  Palindromic composite numbers starting with a digit 7.
 * https://oeis.org/A222728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 1038, u64 n <= 1038,
 *            u128 n <= 1038, big any n
 * Validated: standard, pass, 57 terms through n = 1036 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = nth palindromic composite number (base 10) starting with digit 7.
 * The algorithm enumerates palindromes by length, builds only those whose first digit
 * is 7, checks composite status with is_prime_big, and returns the n-th composite.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

/* Build a palindrome from a half (first ceil(len/2) digits). */
static seq_t mk_palindrome(seq_t half, idx_t len)
{
    idx_t h = (len + 1) / 2;          /* digits in the first half             */
    if (len & 1) {                    /* odd length: drop middle digit of half */
        seq_t rev = reverse_digits(half / 10);
        return half * pow10(h - 1) + rev;
    } else {                          /* even length: whole half reversed      */
        seq_t rev = reverse_digits(half);
        return half * pow10(h) + rev;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 – behaviour below is irrelevant */

    idx_t found = 0;  /* how many composite palindromes with leading 7 have been seen */

    for (idx_t len = 2; ; ++len) {            /* palindrome length starts at 2 */
        idx_t h = (len + 1) / 2;              /* digits in first half */
        seq_t pow_h_1 = pow10(h - 1);         /* 10^(h-1)  (seq_t)          */
        idx_t start = 7 * (idx_t)pow_h_1;     /* smallest half with first digit 7 */
        seq_t pow_h = pow10(h);              /* 10^h  (seq_t) */
        idx_t end   = 8 * (idx_t)pow_h_1 - 1; /* largest half with first digit 7  */

        for (idx_t half = start; half <= end; ++half) {
            seq_t pal = mk_palindrome((seq_t)half, len);
            if (!is_prime_big(pal)) {        /* composite test */
                ++found;
                if (found == n)
                    return pal;
            }
        }
    }
}
