/*
 * A119356  Numbers k such that A000330(k) is squarefree.
 * https://oeis.org/A119356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 2216, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9228 (2026-11-03)
 * Helpers:   is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth integer k for which the sum of squares
   1²+2²+…+k² = k*(k+1)*(2k+1)/6 is squarefree.
   Offset: 1.  The routine memoises terms and works for any index order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
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

seq_t a(idx_t n)
{
    /* static memoisation state ------------------------------------- */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t next_k = 1;   /* candidate k to test */

    if (n == 0) {               /* offset is 1 – undefined for n=0 */
        return 0;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        seq_t k = (seq_t) next_k;
        /* sum of squares 1^2 + … + k^2 = k*(k+1)*(2k+1)/6 */
        seq_t part1 = k * (k + 1);
        seq_t part2 = part1 * (2 * k + 1);
        seq_t sumsq  = part2 / 6;          /* exact division */

        if (is_squarefree(sumsq)) {
            grow_seq(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = next_k;
        }
        ++next_k;
    }

    return terms[n - 1];   /* zero‑based array, n is 1‑based */
}
