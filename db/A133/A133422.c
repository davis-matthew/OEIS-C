/*
 * A133422  Number of steps to reach 1 under repeated applications of the
 *          "7x+1" map of A133421, or -1 if 1 is never reached.
 * https://oeis.org/A133422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of steps to reach 1 under the 7x+1 map: a(1) = 0; a(n) =
 *            # of iterations of f until x == 1, f(x) = x/2 if x even; x/3 if
 *            3|x; x/5 if 5|x; otherwise 7x+1. Return (seq_t)-1 if 1 is never
 *            reached
 * Range:     u8 n <= 30, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *path = NULL;      /* buffer of visited values in current run   */
static size_t path_cap = 0;     /* capacity of the buffer                   */
static size_t path_len = 0;     /* number of used entries */

/* helper to ensure buffer capacity */
static void ensure_path_cap(size_t needed)
{
    if (path_cap < needed) {
        size_t new_cap = path_cap ? (path_cap << 1) : 32;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(path, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory abort */
        path = tmp;
        path_cap = new_cap;
    }
}

/* Computes a(n) */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    if (x == (seq_t)1) return (seq_t)0;  /* a(1) = 0 */

    /* reset path buffer */
    path_len = 0;

    seq_t steps = (seq_t)0;

    /* pragma: loop limit to defensively avoid infinite loops */
    const seq_t MAX_STEPS = (seq_t)100000;

    while (1) {
        /* cycle detection: linear search in path */
        for (size_t i = 0; i < path_len; ++i) {
            if (path[i] == x) {
                return (seq_t)-1;   /* cycle without 1 */
            }
        }

        /* store current value */
        ensure_path_cap(path_len + 1);
        path[path_len++] = x;

        /* apply the 7x+1 map */
        if ((x & (seq_t)1) == (seq_t)0) {
            x = x / (seq_t)2;
        } else if (x % (seq_t)3 == (seq_t)0) {
            x = x / (seq_t)3;
        } else if (x % (seq_t)5 == (seq_t)0) {
            x = x / (seq_t)5;
        } else {
            x = (seq_t)7 * x + (seq_t)1;
        }

        ++steps;
        if (steps > MAX_STEPS) return (seq_t)-1;  /* safety cut‑off */

        if (x == (seq_t)1) return steps;
    }
}
