/*
 * A067271  Numbers n such that the digits of P_7(n), the n-th heptagonal
 *          number, end in n.
 * https://oeis.org/A067271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer m such that P_7(m) = m(5m-3)/2
 *            ends with the decimal digits of m
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 33, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static idx_t scan = 1;     /* next candidate to test */

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        seq_t m = (seq_t)scan;          /* candidate number */
        int d = num_digits_idx(scan);       /* number of decimal digits of m */
        seq_t mod = pow10((idx_t)d);    /* 10^d as seq_t */
        seq_t p   = m * ((seq_t)5 * m - 3) / 2; /* P_7(m) */

        if (p % mod == m) {             /* last d digits equal m */
            append_candidate(&cache, &len, &cap, m);
        }
        scan++;                          /* move on to next candidate */
    }

    /* Sequence is 1-indexed: a(1) is the first term. */
    return cache[n - 1];
}
