/*
 * A106865  Primes of the form 2x^2 + 2xy + 3y^2.
 * https://oeis.org/A106865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9902 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search of primes of the form 2x² + 2xy + 3y² (OEIS A106865). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Dynamically grow the array of found primes */
static void add_prime(uint64_t **arr, size_t *cap, size_t *cnt, uint64_t val)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(**arr));
        if (!tmp) return;           /* allocation failed – caller will free on exit */
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*cnt)++] = val;
}

seq_t a(idx_t n) {
    /* static state to memoize previously generated primes */
    static uint64_t *prime_list = NULL;
    static size_t     prime_cap  = 0;
    static size_t     prime_cnt  = 0;
    static uint64_t   last_checked = 0;   /* last prime value examined */

    /* generate primes of the desired form until we have at least n of them */
    while (prime_cnt < (size_t)n) {
        uint64_t cand;
        if (last_checked == 0) {
            cand = 2;                    /* start with 2 */
        } else {
            cand = last_checked + 1;      /* next odd candidate after the last */
        }

        bool ok = false;
        if (cand == 2) {
            ok = true;                    /* 2 is always included */
        } else if ((cand % 20 == 3) || (cand % 20 == 7)) {
            if (is_prime_u64(cand))
                ok = true;
        }

        if (ok)
            add_prime(&prime_list, &prime_cap, &prime_cnt, cand);

        last_checked = cand;
        /* advance to the next odd number */
        cand += (cand == 2) ? 1 : 2;
    }

    /* now prime_cnt >= n, return the n-th value (1‑based) */
    return (seq_t)prime_list[n - 1];
}
