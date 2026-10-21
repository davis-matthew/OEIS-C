/*
 * A063822  Growth series for fundamental group of orientable closed surface
 *          of genus 12.
 * https://oeis.org/A063822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) - 47*a(n-24) + a(n-25) + b(n), where b(0)=1,
 *            b(1)=1, b(24)=-1, b(25)=-1, and b(n)=0 otherwise. The first term
 *            is a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute the n‑th term of OEIS A063822. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;

    /* One‑time allocation and initialization of a(0). */
    if (!seq) {
        cap = 64;
        seq = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
        seq[0] = (seq_t)1;   /* a(0) = 1 */
        max_computed = 0;
    }

    /* Grow the array if necessary. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq = tmp;
        cap = new_cap;
    }

    /* Compute terms up to n (inclusive). */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = 0;

        if (i >= 1)      val += (seq_t)47 * seq[i - 1];
        if (i >= 24)     val -= (seq_t)47 * seq[i - 24];
        if (i >= 25)     val += seq[i - 25];

        /* Add the source‑term constants. */
        if (i == 0)      val += (seq_t)1;   /* not reached, a(0) is preset */
        else if (i == 1) val += (seq_t)1;
        else if (i == 24) val -= (seq_t)1;
        else if (i == 25) val -= (seq_t)1;

        seq[i] = val;
    }

    max_computed = n;
    return seq[n];
}
