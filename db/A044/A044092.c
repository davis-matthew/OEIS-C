/*
 * A044092  Numbers n such that string 3,0 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044092: numbers n where the base‑5 representation contains "30",
   but the representation of n‑1 does not.  The nth term is computed by
   generating terms in increasing order and caching them. */
#include <stdbool.h>
#include <stdlib.h>

/* Check if a number contains the consecutive digits 3,0 in its base‑5 representation. */
static bool has30(uint64_t x)
{
    unsigned char digits[64];
    int len = 0;

    if (x == 0) {
        digits[len++] = 0;
    } else {
        while (x > 0) {
            digits[len++] = (unsigned char)(x % 5);
            x /= 5;
        }
    }

    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 3 && digits[i - 1] == 0)
            return true;
    }
    return false;
}

/* Return the nth term of sequence A044092 (1‑based index). */
seq_t a(idx_t n)
{
    static idx_t generated = 0;          /* number of terms already in cache (0‑based) */
    static uint64_t current = 1;         /* next integer to test */
    static seq_t *terms = NULL;          /* cached terms */
    static size_t cap = 0;               /* allocated capacity */

    if (n == 0)
        return (seq_t)0;                 /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (generated < n) {
        if (has30(current) && !has30(current - 1)) {
            /* Grow cache if needed. */
            if (generated >= cap) {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
                cap = new_cap;
            }
            terms[generated] = (seq_t)current;
            ++generated;
        }
        ++current;
    }

    return terms[n - 1];
}
