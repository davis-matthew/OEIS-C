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
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>          /* for size_t  */
#include "../utils/isPrimeGeneric.h"

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
