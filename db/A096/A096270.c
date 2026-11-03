/*
 * A096270  Fixed point of the morphism 0->01, 1->011.
 * https://oeis.org/A096270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9630 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A096270 (finite morphism fixed‑point)
 * a(0) = 0, a(1) = 1, a(2) = 0, a(3) = 1, ...
 * The sequence is the limit of the morphism
 *   g(0) → 0 1
 *   g(1) → 0 1 1
 *
 * The term a(n) is obtained by generating the word
 *   w₀ = 0
 *   w_{k+1} = g(w_k)
 * until |w_k| > n and then returning w_k[n].
 * This runs in O(n) time and O(n) memory for any n.
 */

#include <stdlib.h>   /* malloc, free */
#include "../utils/mem.h"

static seq_t *word = NULL;      /* cached word w_k  */
static size_t word_len = 0;     /* current length */

/* generate the fixed‑point word up to index n (inclusive) */
static void ensure_word(idx_t n)
{
    if (word_len > (size_t)n)
        return;                /* already sufficient */

    /* initialisation */
    if (!word) {
        word_len = 1;
        word = mem_new(seq_t, word_len);
        word[0] = (seq_t)0;
    }

    /* expand until we have at least n+1 symbols */
    while ((size_t)n >= word_len) {
        /* count numbers of 0 and 1 in the current word */
        size_t cnt0 = 0, cnt1 = 0;
        for (size_t i = 0; i < word_len; i++)
            (word[i] == 0) ? ++cnt0 : ++cnt1;

        /* new length: 0→2 symbols, 1→3 symbols */
        size_t newlen = 2 * cnt0 + 3 * cnt1;
        seq_t *next = mem_new(seq_t, newlen);

        /* expand each symbol */
        size_t pos = 0;
        for (size_t i = 0; i < word_len; i++) {
            if (word[i] == 0) {          /* 0 → 0 1 */
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
            } else {                     /* 1 → 0 1 1 */
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)1;
            }
        }

        mem_free(word);
        word      = next;
        word_len  = newlen;
    }
}

/* public sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) = 0 by definition */

    ensure_word(n);
    return word[(size_t)n];
}
