/*
 * A175970  Complement of A051349(n), where A051349(n) = the lexicographically
 *          earliest sequence with first differences as increasing sequence of
 *          composites A002808(n).
 * https://oeis.org/A175970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   ensure_cap_bool, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175970: Complement of A051349.
   The lexicographically earliest sequence with increasing composite
   differences is generated; a boolean array marks its terms.
   Numbers not in that set are stored as the complement sequence.  */

/* The helper function is_prime() is supplied by the build environment.
   Include its header. */
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/ensure_cap_bool.h"

/* Grow a seq_t array to contain at least 'need' elements. */
static void ensure_cap_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (need < *cap)
        return;
    size_t new_cap = *cap ? *cap : 64;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    *arr = tmp;
    *cap = new_cap;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    static bool *inS = NULL;
    static size_t inS_cap = 0;

    static idx_t last_s = 1;          /* last term of S */
    static idx_t d_prev = 0;          /* previous composite difference */
    static idx_t max_s = 1;           /* largest S term generated so far */

    static idx_t cur_val = 1;         /* current integer to test for complement */

    static seq_t *comp = NULL;
    static size_t comp_len = 0, comp_cap = 0;

    /* Initialization on first call. */
    if (!inS) {
        inS_cap = 16;
        inS = (bool *)calloc(inS_cap, sizeof(bool));
        if (!inS)
            abort();
        inS[1] = true;           /* S starts with 1 */
    }
    if (!comp) {
        comp_cap = 64;
        comp = (seq_t *)malloc(comp_cap * sizeof(seq_t));
        if (!comp)
            abort();
    }

    /* Generate complement values until we have at least n of them. */
    while (comp_len < n) {
        /* Generate S terms until we reach or surpass cur_val. */
        while (max_s < cur_val) {
            /* Find next composite > d_prev. */
            idx_t next_d = d_prev + 1;
            while (next_d <= 1 || is_prime(next_d))
                ++next_d;
            d_prev = next_d;
            last_s += next_d;
            if (last_s > max_s)
                max_s = last_s;
            ensure_cap_bool(&inS, &inS_cap, last_s);
            inS[last_s] = true;
        }

        /* If cur_val is not in S, it belongs to the complement. */
        if (!inS[(size_t)cur_val]) {
            ensure_cap_seq(&comp, &comp_cap, comp_len + 1);
            comp[comp_len++] = cur_val;
        }
        ++cur_val;
    }

    return comp[n - 1];
}
