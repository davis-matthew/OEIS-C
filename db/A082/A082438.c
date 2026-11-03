/*
 * A082438  Palindromic primes with middle digit 2.
 * https://oeis.org/A082438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth palindromic prime whose middle digit is 2. The
 *            value is obtained by enumerating odd‑length palindromes with
 *            middle digit 2 and testing each for primality
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 140, u64 n <= 140, u128 n <= 140,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 139 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>          /* for size_t  */
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Computes 10^digits as a seq_t value. */
static inline seq_t pow10_seq_t(int digits)
{
    seq_t p = 1;
    for (int i = 0; i < digits; ++i)
        p *= 10;
    return p;
}

/* Builds the palindrome L[...][2][reverse(L)] where L is the left part. */
static inline seq_t make_pal(seq_t L)
{
    /* Count digits of L. */
    int d = 0;
    seq_t tmp = L;
    if (tmp == 0)
        d = 1;          /* handle L = 0: the digit count is 1 */
    else
        while (tmp) {
            ++d;
            tmp /= 10;
        }

    /* Reverse L. */
    seq_t rev = 0, t = L;
    while (t) {
        rev = rev * 10 + (t % 10);
        t /= 10;
    }

    /* Construct palindrome: L · 10 + 2, then append reverse(L). */
    seq_t p = L;
    p = p * 10 + 2;
    p = p * pow10_seq_t(d) + rev;
    return p;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, return 0 */

    /* Static cache for already generated terms. */
    static seq_t cache[20000];      /* sufficient for n up to 20000 */
    static size_t cache_len = 0;    /* number of terms cached so far */
    static seq_t nextL = 1;         /* left part to try next */

    /* Initialise the cache with the first term 2. */
    if (cache_len == 0) {
        cache[0] = 2;
        cache_len = 1;
    }

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        seq_t cand = make_pal(nextL++);
        if (isPrimeGeneric(cand))
            cache[cache_len++] = cand;
    }

    return cache[(size_t)n - 1];
}
