/*
 * A152242  Integers formed by concatenating primes.
 * https://oeis.org/A152242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer that can be represented as a
 *            concatenation of one or more primes in base‑10. Computation is
 *            performed by a simple search: numbers are inspected in
 *            increasing order and accepted when a valid prime concatenation
 *            is found
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/grow_seq.h"   /* void grow_seq(seq_t **arr, size_t *cap, size_t need); */

static bool can_concat(seq_t n) {
    /* Convert n to a decimal digit array (most significant digit first). */
    unsigned digits[256];
    int len = 0;
    seq_t tmp = n;
    if (tmp == (seq_t)0) {
        digits[len++] = 0;
    } else {
        while (tmp > (seq_t)0) {
            digits[len++] = (unsigned)(tmp % (seq_t)10);
            tmp /= (seq_t)10;
        }
        /* Reverse to get most significant first. */
        for (int i = 0; i < len / 2; ++i) {
            unsigned t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* DP: memo[pos] == true means suffix starting at 'pos' can be split into primes. */
    bool memo[257];
    for (int i = 0; i <= len; ++i) memo[i] = false;
    memo[len] = true; /* empty suffix is valid */

    for (int pos = len - 1; pos >= 0; --pos) {
        if (digits[pos] == 0) {          /* leading zero in a part is forbidden */
            memo[pos] = false;
            continue;
        }
        seq_t sub = 0;
        bool good = false;
        for (int j = pos + 1; j <= len; ++j) {
            sub = sub * (seq_t)10 + (seq_t)digits[j - 1];
            if (sub > (seq_t)UINT64_MAX) break;   /* cannot test as idx_t */
            if (is_prime((idx_t)sub) && memo[j]) {
                good = true;
                break;
            }
        }
        memo[pos] = good;
    }
    return memo[0];
}

seq_t a(idx_t n) {
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t next_candidate = 0; /* next integer to test */

    if (cache == NULL) {
        cap = 0;
        len = 0;
        cache = NULL;
    }

    /* Build terms until we have at least n of them. */
    while (len < n) {
        if (len == 0 && next_candidate == 0) next_candidate = (seq_t)2;
        /* Find next number that can be concatenated from primes. */
        while (!can_concat(next_candidate)) {
            next_candidate = next_candidate + (seq_t)1;
        }
        /* Store the found term. */
        grow_seq(&cache, &cap, len + 1);
        cache[len] = next_candidate;
        len++;

        /* Prepare candidate for the next search. */
        next_candidate = next_candidate + (seq_t)1;
    }

    return cache[n - 1];
}
