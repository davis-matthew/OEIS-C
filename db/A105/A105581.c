/*
 * A105581  Primes whose indices are palindromic.
 * https://oeis.org/A105581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = prime(p) where p is the n‑th palindromic integer
 *            (starting at 1)
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_trial.h"

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

static idx_t reverse_without_last(idx_t x, int odd) {
    idx_t rev = 0;
    if (odd) x /= 10;          /* skip the middle digit for odd length */
    while (x > 0) {
        rev = rev * 10 + x % 10;
        x /= 10;
    }
    return rev;
}

static idx_t make_pal(idx_t half, int odd) {
    idx_t pal = half;
    idx_t rev = reverse_without_last(half, odd);
    while (rev > 0) {
        pal = pal * 10 + rev % 10;
        rev /= 10;
    }
    return pal;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */

    idx_t count = 0;
    for (int len = 1; ; ++len) {
        if (len == 1) {
            /* 1–9 are the 1‑digit palindromes */
            for (idx_t half = 1; half <= 9; ++half) {
                ++count;
                if (count == n) return get_prime(half);
            }
        } else {
            int half_len = (len + 1) / 2;
            idx_t start = 1;
            for (int i = 1; i < half_len; ++i) start *= 10;   /* first half cannot start with 0 */
            idx_t end = start * 10 - 1;
            int odd = len % 2;   /* 1 if odd, 0 if even */
            for (idx_t half = start; half <= end; ++half) {
                idx_t pal = make_pal(half, odd);
                ++count;
                if (count == n) return get_prime(pal);
            }
        }
    }
}
