/*
 * A049354  Digitally balanced numbers in base 3: equal numbers of 0's, 1's,
 *          2's.
 * https://oeis.org/A049354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1184, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   a49_balanced
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force enumeration: a(n) is the nth positive integer whose
 * base‑3 representation contains an equal number of digits 0, 1 and 2.
 * The sequence is generated lazily and cached across calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a49_balanced.h"

static seq_t *a49_coef = NULL;     /* cache of computed terms          */
static size_t a49_len   = 0;        /* number of cached terms           */
static size_t a49_cap   = 0;        /* current allocation capacity      */
static idx_t  a49_next  = 1;        /* next candidate integer to test   */

/* enlarge the cache if necessary */
static void a49_ensure_cap(size_t need)
{
    if (need <= a49_cap) return;
    size_t new_cap = a49_cap == 0 ? 8 : a49_cap * 2;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(a49_coef, new_cap * sizeof(seq_t));
    if (!new_buf) exit(1);        /* terminate on allocation failure     */
    a49_coef = new_buf;
    a49_cap  = new_cap;
}

/* public interface required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* offset is 1; lower indices are unused */

    /* generate terms until we have at least n of them */
    if (a49_len < n) {
        a49_ensure_cap(n);
        while (a49_len < n) {
            /* find the next balanced integer */
            while (!a49_balanced(a49_next)) ++a49_next;
            /* store the balanced integer */
            a49_coef[a49_len++] = (seq_t)a49_next;
            ++a49_next;          /* start searching from the next number   */
        }
    }
    return a49_coef[n - 1];
}
