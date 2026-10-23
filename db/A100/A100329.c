/*
 * A100329  a(n) = -a(n-1) -a(n-2) -a(n-3) +a(n-4), a(0)=0, a(1)=1, a(2)=-1,
 *          a(3)=0.
 * https://oeis.org/A100329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - a(n-2) - a(n-3) + a(n-4), with a(0)=0, a(1)=1,
 *            a(2)=-1, a(3)=0. The function computes all terms on demand and
 *            caches them
 * Range:     i8 n <= 22, i16 n <= 45, i32 n <= 88, i64 n <= 175,
 *            i128 n <= 349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for computed terms */
static seq_t *seq = NULL;
static size_t capacity = 0;   /* allocated size (number of elements)   */
static size_t computed  = 0;   /* number of terms that are already filled (0‑based) */

/* make sure we have terms up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    /* allocate or grow the array if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity > 0 ? capacity : 4;
        while (new_cap <= (size_t)n + 1) {
            new_cap *= 2;
            if (new_cap == 0) new_cap = 1;  /* protect against overflow */
        }
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* initialise the first four terms the first time */
    if (computed == 0) {
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)-1;
        seq[3] = (seq_t)0;
        computed = 4;
    }

    /* compute new terms if we haven't reached n yet */
    if (computed <= (size_t)n) {
        for (size_t i = computed; i <= (size_t)n; ++i) {
            seq[i] = -seq[i-1] - seq[i-2] - seq[i-3] + seq[i-4];
        }
        computed = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return seq[(size_t)n];
}
