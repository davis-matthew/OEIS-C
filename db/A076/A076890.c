/*
 * A076890  Number of primes up to n-th palindromic number.
 * https://oeis.org/A076890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of primes <= nth palindromic number (i.e. a(n) =
 *            A000720( A002113(n) ))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   extend_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/extend_sieve.h"

/* ---------- helper: power of 10 (int <= 10) ---------- */
static uint32_t pow10_int(int e) {
    uint32_t r = 1u;
    while (e-- > 0) r *= 10u;
    return r;
}

/* ---------- helper: nth palindromic number (1‑based) ---------- */
static uint32_t nth_palindrome(idx_t n) {
    if (n == 0) return 0u;           /* undefined, return 0 */
    /* find length of the palindrome */
    idx_t remaining = n;
    int len = 0;
    while (1) {
        ++len;
        int half = (len + 1) / 2;
        uint32_t count = 9u * pow10_int(half - 1);
        if (len % 2 == 0 && len == 2) count = 9u; /* first even length */
        if (remaining <= count) break;
        remaining -= count;
    }
    int half = (len + 1) / 2;
    uint32_t start = pow10_int(half - 1);
    uint32_t base = start + (uint32_t)(remaining - 1);
    /* build the palindrome by mirroring the base digits */
    uint32_t pal = base;
    uint32_t rev = (len & 1) ? base / 10 : base;
    while (rev > 0) {
        pal = pal * 10u + (rev % 10u);
        rev /= 10u;
    }
    return pal;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n) {
    static uint32_t *pi_prefix = NULL;
    static size_t cur_limit = 0;   /* maximum limit sieve has been built for */

    /* compute nth palindrome (fits in 32 bits for test ranges) */
    uint32_t pal = nth_palindrome(n);
    if (pal == 0u) return (seq_t)0;   /* safety for n==0 */

    /* extend sieve if needed */
    extend_sieve(pal, &pi_prefix, &cur_limit);
    if (pi_prefix == NULL) return (seq_t)0;   /* allocation failure */

    return (seq_t)pi_prefix[pal];
}
