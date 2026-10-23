/*
 * A044493  Numbers n such that string 1,4 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer n such that base‑6
 *            representation of n contains substring 14 but n+1 does not.
 *            (OEIS A044493)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether the base‑6 representation of x contains the substring “14”.
 * We process digits from least significant to most significant, keeping
 * the previously processed digit. A pair prev = 4 and d = 1 indicates
 * the substring 14 (more significant 1, less significant 4). */
static bool has14(seq_t x) {
    seq_t prev = 0;
    bool first = true;
    while (x > 0) {
        seq_t d = x % 6;
        if (!first && prev == 4 && d == 1)
            return true;
        first = false;
        prev = d;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *terms = NULL;   /* array of found terms */
    static idx_t term_count = 0; /* number of terms stored */
    static idx_t capacity = 0;   /* allocated capacity of terms array */
    static seq_t candidate = 1;  /* next integer to test */

    /* Grow the cache if needed */
    if (!terms) {
        capacity = 1024;
        terms = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!terms) return 0; /* allocation failed */
    }

    /* Find more terms until we have at least n of them */
    while (term_count < n) {
        if (has14(candidate) && !has14(candidate + 1)) {
            /* Append candidate to cache */
            if (term_count == capacity) {
                capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(terms, sizeof(seq_t) * capacity);
                if (!tmp) return 0; /* allocation failed */
                terms = tmp;
            }
            terms[term_count++] = candidate;
        }
        candidate++;
    }

    return terms[n - 1]; /* OEIS offset 1 */
}
