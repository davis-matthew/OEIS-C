/*
 * A077306  Smallest multiple of n formed by the concatenation of n successive
 *          numbers, or 0 if no such number exists.
 * https://oeis.org/A077306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 310 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: smallest multiple of n formed by concatenation of n successive integers. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

/* Ensure pow10[0..needed] exists; pow10[i] = 10^i mod n. */
static void ensure_pow10mod(idx_t n, idx_t **pow10, size_t *cap, size_t needed)
{
    if (*cap == 0) {
        *cap = 1;
        *pow10 = (idx_t*)malloc(sizeof(idx_t));
        if (!*pow10) exit(1);
        (*pow10)[0] = 1 % n;
    }
    while (*cap <= needed) {
        size_t newcap = (*cap) * 2;
        idx_t *tmp = (idx_t*)realloc(*pow10, newcap * sizeof(idx_t));
        if (!tmp) exit(1);
        for (size_t i = *cap; i < newcap; ++i)
            tmp[i] = (tmp[i - 1] * 10ULL) % n;
        *pow10 = tmp;
        *cap = newcap;
    }
}

/* fast integer digit length, used for n up to 2^64-1 */
static inline int digit_len(idx_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Build the full concatenated number [start, ..., start+n-1]. */
static seq_t build_value(idx_t start, idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        idx_t num = start + i;
        int d = digit_len(num);
        seq_t pow10d = ipow_i32((seq_t)10, d);
        val = val * pow10d + (seq_t)num;
    }
    return val;
}

/*
 * External linkage, as required.  The function works for any seq_t/idx_t
 * definition; it returns 0 when no such concatenation exists within a
 * reasonable bound.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, safe guard */

    /* Pre‑allocate powers of ten modulo n up to an overestimate. */
    size_t maxlen = (size_t)n * 10ULL + 10;      /* safe upper bound */
    size_t powcap = 0;
    idx_t *pow10 = NULL;
    ensure_pow10mod(n, &pow10, &powcap, maxlen);

    /* Initial window starting at 1. */
    idx_t start = 1;
    idx_t len_total = 0;
    idx_t mod_val = 0;
    for (idx_t i = 0; i < n; ++i) {
        idx_t num = start + i;
        int d = digit_len(num);
        if ((size_t)d > powcap - 1)
            ensure_pow10mod(n, &pow10, &powcap, (size_t)d);
        mod_val = (mod_val * pow10[d] + (num % n)) % n;
        len_total += d;
    }
    if (mod_val == 0) {
        seq_t res = build_value(start, n);
        free(pow10);
        return res;
    }

    /* Slide the window; stop after a generous bound. */
    const size_t MAX_ITER = 10 * n + 1000;
    for (size_t iter = 1; iter < MAX_ITER; ++iter) {
        ++start;

        /* Remove the leftmost number. */
        idx_t num_remove = start - 1;
        int d_remove = digit_len(num_remove);
        idx_t len_rest = len_total - d_remove;
        if ((size_t)len_rest > powcap - 1)
            ensure_pow10mod(n, &pow10, &powcap, (size_t)len_rest);
        idx_t term_mod_remove = ((num_remove % n) * pow10[len_rest]) % n;
        mod_val = (mod_val + n - term_mod_remove) % n;

        /* Add the new rightmost number. */
        idx_t num_add = start + n - 1;
        int d_add = digit_len(num_add);
        if ((size_t)d_add > powcap - 1)
            ensure_pow10mod(n, &pow10, &powcap, (size_t)d_add);
        mod_val = (mod_val * pow10[d_add] + (num_add % n)) % n;
        len_total = len_rest + d_add;

        if (mod_val == 0) {
            seq_t res = build_value(start, n);
            free(pow10);
            return res;
        }
    }

    free(pow10);
    return (seq_t)0;   /* no such number found within the bound */
}
