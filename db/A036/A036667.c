/*
 * A036667  Numbers of the form 2^i*3^j, i+j even.
 * https://oeis.org/A036667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A036667 – the nth number of the form 2^i · 3^j with i+j even
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 175, u64 n <= 672,
 *            u128 n <= 2636, big any n
 * Helpers:   parity_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/parity_even.h"

/* ---------- static cache data ------------------------------------- */
static seq_t *humble = NULL;     /* all 2^i*3^j numbers, ascending */
static size_t hum_cap = 0, hum_len = 0;
static size_t p2 = 0, p3 = 0;    /* pointers for the humble‑number algorithm */

static seq_t *evens = NULL;      /* the required sequence (i+j even) */
static size_t evens_cap = 0, evens_len = 0;

/* ---------- helper: grow a seq_t array ------------------------------ */
static void grow_array(seq_t **ptr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 32 : (*cap * 2);
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *ptr = tmp;
    *cap = new_cap;
}

/* ---------- ensure that the cache contains at least n terms -------- */
static void ensure_cache(idx_t n)
{
    while (evens_len < (size_t)n) {
        /* generate the next humble number */
        seq_t next2 = humble[p2] * (seq_t)2;
        seq_t next3 = humble[p3] * (seq_t)3;
        seq_t next  = (next2 < next3) ? next2 : next3;

        if (next == next2) ++p2;
        if (next == next3) ++p3;

        /* store the new humble number */
        grow_array(&humble, &hum_cap, hum_len + 1);
        humble[hum_len++] = next;

        /* if its exponent sum is even, add it to the answer list */
        if (parity_even(next)) {
            grow_array(&evens, &evens_cap, evens_len + 1);
            evens[evens_len++] = next;
        }
    }
}

/* ---------- public function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (hum_len == 0) {
        grow_array(&humble, &hum_cap, 1);
        humble[0] = (seq_t)1;
        hum_len = 1;
        p2 = p3 = 0;

        grow_array(&evens, &evens_cap, 1);
        evens[0] = (seq_t)1;      /* 2^0·3^0 satisfies i+j even */
        evens_len = 1;
    }

    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    ensure_cache(n);
    return evens[(size_t)n - 1];
}
