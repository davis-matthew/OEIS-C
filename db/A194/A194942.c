/*
 * A194942  List of seven normalized ternary Abelian squarefree words.
 * https://oeis.org/A194942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabf
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   word_violates
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code: a(n) = the nth letter in the lexicographically ordered list of
   normalized ternary Abelian‑square‑free words.  The words are concatenated
   in increasing order of length.  This implementation generates words
   on demand and appends their letters to a global buffer. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/word_violates.h"

/* External helper types supplied by the build system. */
extern seq_t a(idx_t n);               /* function to implement */

/* ------------------------------------------------------------------ */
/* Internal data structures                                           */
/* ------------------------------------------------------------------ */

#define MAX_LEN 256                     /* maximum word length expected */

static seq_t *flat = NULL;              /* flattened output buffer   */
static size_t flat_len = 0;              /* number of letters stored */
static size_t flat_cap = 0;              /* capacity of the buffer    */
static idx_t generated_len = 0;          /* maximum word length fully generated */

/* ------------------------------------------------------------------ */
/* Utility functions and helpers                                     */
/* ------------------------------------------------------------------ */

/* Ensure the flattened buffer can hold at least "need" more letters. */
static void ensure_flat_capacity(size_t need)
{
    if (flat_len + need > flat_cap) {
        size_t new_cap = flat_cap ? flat_cap * 2 : 16;
        if (new_cap < flat_len + need) new_cap = flat_len + need;
        flat = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
        flat_cap = new_cap;
    }
}

/* Append a letter to the flattened buffer. */
static void flat_append(seq_t val)
{
    ensure_flat_capacity(1);
    flat[flat_len++] = val;
}

/* Recursive generation of all words of length L.  Parameters:
   L    - target length
   pos  - current position
   seen1   true if symbol 1 has appeared before pos
   seen2   true if symbol 2 has appeared before pos
   buf   working buffer holding the word so far */
static void gen_rec(int L, int pos, bool seen1, bool seen2, seq_t *buf)
{
    if (pos == L) { /* finished word */
        for (int i = 0; i < L; ++i)
            flat_append(buf[i]);
        return;
    }

    if (pos == 0) { /* first letter must be 0 */
        buf[pos] = (seq_t)0;
        gen_rec(L, pos + 1, false, false, buf);
        return;
    }

    /* try all three symbols in lexicographic order */
    for (int cand = 0; cand <= 2; ++cand) {
        if (cand == 2 && !seen1) continue;      /* normalization: 2 after 1 */
        buf[pos] = (seq_t)cand;
        if (!word_violates(pos + 1, buf)) {
            bool nseen1 = seen1 || (cand == 1);
            bool nseen2 = seen2 || (cand == 2);
            gen_rec(L, pos + 1, nseen1, nseen2, buf);
        }
    }
}

/* Generate all words of length L and append their letters to the
   global flattened buffer. */
static void generate_length(int L)
{
    seq_t word[MAX_LEN];
    gen_rec(L, 0, false, false, word);
}

/* Generate flattened letters until at least target terms are available. */
static void generate_to(size_t target)
{
    while (flat_len < target) {
        ++generated_len;
        generate_length((int)generated_len);
    }
}

/* ------------------------------------------------------------------ */
/* Public API                                                           */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for n=0, so return 0 */

    generate_to((size_t)n);
    return flat[n - 1];
}
