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
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- prime sieve with prefix prime counts ---------- */
static void extend_sieve(uint32_t new_limit,
                         uint32_t **pi_pref, size_t *cur_limit) {
    if (new_limit <= *cur_limit) return;

    /* allocate prefix array of required size */
    uint32_t *pi = (uint32_t*)realloc(*pi_pref,
                                     (new_limit + 1) * sizeof(uint32_t));
    if (!pi) {
        /* allocation failure – abort by returning 0 which will be
           handled by the calling function as an error value */
        *pi_pref = NULL;
        return;
    }
    *pi_pref = pi;

    /* temporary prime flag array */
    uint8_t *is_prime = (uint8_t*)malloc(new_limit + 1);
    if (!is_prime) return;

    for (uint32_t i = 0; i <= new_limit; ++i) is_prime[i] = 1;
    if (new_limit >= 0) is_prime[0] = 0;
    if (new_limit >= 1) is_prime[1] = 0;

    for (uint32_t i = 2; i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* fill prefix counts */
    uint32_t cnt = 0;
    for (uint32_t i = 0; i <= new_limit; ++i) {
        if (is_prime[i]) ++cnt;
        pi[i] = cnt;
    }

    free(is_prime);
    *cur_limit = new_limit;
}

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
