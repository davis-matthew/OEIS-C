/*
 * A229879  Palindromic prime numbers == 5 (mod 9).
 * https://oeis.org/A229879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   compute the nth palindromic prime congruent to 5 mod 9 by
 *            brute‑force search over odd‑length palindromes (length 1, 3, 5,
 *            …)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 993, u64 n <= 993,
 *            u128 n <= 993, big any n
 * Validated: standard, pass, 50 terms through n = 982 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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
    /* handle 1‑digit palindromes */
    idx_t count = 0;
    for (seq_t d = (seq_t)2; d <= (seq_t)9; ++d) {
        seq_t val = d;
        if (val % (seq_t)9 == (seq_t)5 && is_prime_big(val)) {
            ++count;
            if (count == n) return val; /* 5 is the only 1‑digit candidate */
        }
    }

    /* generate palindromes of odd length 3,5,7,… */
    for (idx_t k = 1; ; ++k) {
        seq_t pow10_k   = pow10(k);        /* 10^k      */
        seq_t pow10_k1  = pow10(k + 1);    /* 10^(k+1)  */
        seq_t start = pow10_k;             /* first i of this length */
        seq_t end   = pow10_k1 - (seq_t)1; /* last i of this length */

        for (seq_t i = start; i <= end; ++i) {
            seq_t rev = reverse_digits(i / (seq_t)10);          /* reverse left part */
            seq_t val = i * pow10_k + rev;                  /* full palindrome */
            if (val % (seq_t)9 == (seq_t)5 && is_prime_big(val)) {
                ++count;
                if (count == n) return val;
            }
        }
    }

    /* should never reach here */
    return (seq_t)0;
}
