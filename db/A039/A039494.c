/*
 * A039494  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 2's.
 * https://oeis.org/A039494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th number whose base‑12 representation has the same
 *            non‑zero number of 0's and 2's (both counts > 0). This
 *            implementation searches successive integers, testing each for
 *            the condition
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static bool good(seq_t v)
{
    int cnt0 = 0, cnt2 = 0;
    seq_t x = v;
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)12);
        if (d == 0)   ++cnt0;
        else if (d == 2) ++cnt2;
        x /= (seq_t)12;
    }
    return (cnt0 == cnt2) && (cnt0 > 0);
}

static void append(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    ++*len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset, safe return */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t next = 1;   /* next integer to test */

    while (len < n) {
        seq_t v = next++;
        if (good(v))
            append(&cache, &len, &cap, v);
    }
    return cache[n - 1];
}
