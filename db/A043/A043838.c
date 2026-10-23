/*
 * A043838  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 1 mod 6.
 * https://oeis.org/A043838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer k such that the number of runs in
 *            its base‑4 representation is congruent to 1 modulo 6
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *seq = NULL;     /* cached terms */
    static size_t sz   = 0;       /* number of terms stored */
    static size_t cap  = 0;       /* allocated capacity */
    static seq_t next = 1;        /* next candidate to test */

    /* allocate initial storage if necessary */
    if (cap == 0) {
        cap = 64;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend the cache until we have at least n terms */
    while (sz < n) {
        seq_t v = next;
        int runs = 0;
        seq_t prev = (seq_t)-1;   /* sentinel for "none" */

        /* count runs in base‑4 representation of v */
        while (v > 0) {
            seq_t d = v % 4;
            if (d != prev) {
                ++runs;
                prev = d;
            }
            v /= 4;
        }

        /* store the term if it satisfies the condition */
        if (runs % 6 == 1) {
            if (sz >= cap) {
                cap *= 2;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[sz++] = next;
        }
        ++next;
    }

    return seq[n - 1];
}
