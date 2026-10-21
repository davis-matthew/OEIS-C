/*
 * A080945  Numbers having more than two divisors that are also suffixes in
 *          binary representation.
 * https://oeis.org/A080945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_suffix, rec_suffix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080945
   Formula: a(n) is the n-th integer that has at least three divisors,
   each of which appears as a suffix of the integer's binary representation. */
/* The implementation performs a brute‑force scan, testing every positive
   integer until the requested index is reached.  It uses trial division for
   factorisation and enumerates all divisors recursively, counting how many
   of them satisfy the suffix condition.  The sequence is memoised so that
   subsequent calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_suffix.h"
#include "../utils/rec_suffix.h"

/* Checks whether n has at least three suffix divisors. */
static bool qualifies(uint64_t n) {
    /* Factorise n by trial division. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            primes[cnt] = p;
            exps[cnt]   = exp;
            ++cnt;
        }
    }
    if (t > 1) {                    /* remaining prime factor */
        primes[cnt] = t;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Quick reject if n has at most 2 divisors. */
    size_t div_count = 1;
    for (size_t i = 0; i < cnt; ++i) {
        div_count *= (exps[i] + 1);
        if (div_count > 2) break;
    }
    if (div_count <= 2) return false;

    uint8_t suffix_cnt = 0;
    /* Enumerate all divisors and count suffix ones. */
    return rec_suffix(n, primes, exps, 0, cnt, 1, &suffix_cnt);
}

/* Memoisation buffer for the sequence. */
static seq_t *memo = NULL;   /* array of values */
static size_t  memo_len = 0; /* number of values stored */
static size_t  memo_cap = 0; /* capacity of the array */
static uint64_t cur_candidate = 1; /* next integer to test */

/* Ensure the memo array has at least need elements. */
static void ensure_capacity(size_t need) {
    if (need <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        abort(); /* allocation failure */
    }
    memo = new_ptr;
    memo_cap = new_cap;
}

/* Public function required by the test harness. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, but graceful */
    while (memo_len < (size_t)n) {
        cur_candidate++; /* test next integer */
        if (qualifies(cur_candidate)) {
            ensure_capacity(memo_len + 1);
            memo[memo_len++] = (seq_t)cur_candidate;
        }
    }
    return memo[n - 1];
}
