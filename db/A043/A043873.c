/*
 * A043873  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 6 mod 10.
 * https://oeis.org/A043873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043873 – brute force search: a(n) = smallest integer m > a(n‑1)
   with (number of runs in base‑4 representation of m) ≡ 6 (mod 10) */

#include <stdlib.h>

/* Count the number of runs in the base‑4 representation of v. */
static inline int count_runs_base4(seq_t v)
{
    if (v == (seq_t)0) return 1;
    seq_t prev = v % (seq_t)4;
    v /= (seq_t)4;
    int runs = 1;
    while (v != (seq_t)0) {
        seq_t d = v % (seq_t)4;
        if (d != prev) runs++;
        prev = d;
        v /= (seq_t)4;
    }
    return runs;
}

/* Ensure that at least 'need' elements can be stored. */
static inline void ensure_capacity(size_t need)
{
    static size_t array_cap = 0;
    static seq_t *term_arr = NULL;
    if (need <= array_cap) return;
    size_t new_cap = array_cap ? array_cap * 2 : 2;
    while (new_cap < need) new_cap *= 2;
    term_arr = (seq_t *)realloc(term_arr, new_cap * sizeof(seq_t));
    array_cap = new_cap;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */

    /* Static state to allow random access to any n. */
    static size_t term_count = 0;
    static seq_t *term_arr = NULL;
    static size_t array_cap = 0;
    static idx_t next_candidate = 1;

    /* Extend the sequence until we have n terms. */
    while (term_count < n) {
        int runs = count_runs_base4((seq_t)next_candidate);
        if (runs % 10 == 6) {
            if (term_count == array_cap) {
                size_t new_cap = array_cap ? array_cap * 2 : 2;
                while (new_cap <= term_count) new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(term_arr, new_cap * sizeof(seq_t));
                term_arr = tmp;
                array_cap = new_cap;
            }
            term_arr[term_count++] = next_candidate;
        }
        next_candidate++;
    }

    return term_arr[n - 1];
}
