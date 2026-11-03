/*
 * A044214  Numbers k such that string 3,3 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the base‑8 *
 *            representation of k contains the substring "33" while k-1 does
 *            not
 * Range:     u8 n <= 4, u16 n <= 1010, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9662 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return true if x written in base 8 contains "33" as consecutive digits. */
static bool has33_octal(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 8);
        if (prev == 3 && d == 3) return true;
        prev = d;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static state so that non‑increasing calls are fast. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t next_k = 1;   /* next candidate to test */

    if (n == 0) return 0;   /* Undefined for the offset, but safe. */

    /* Generate terms until we have the desired index. */
    while (terms_len < n) {
        if (has33_octal(next_k) && !has33_octal(next_k - 1)) {
            /* Ensure enough space and store the term. */
            grow_seq(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = next_k;
        }
        next_k++;
    }
    return terms[n - 1];
}
