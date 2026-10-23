/*
 * A157129  An analog of the Kolakoski sequence A000002, only now a(n) =
 *          (length of n-th run divided by 2) using 1 and 2 and starting with
 *          1,1.
 * https://oeis.org/A157129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   expansion, seq_realloc, to_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A157129: run lengths of the morphic word
 * (definition via the morphism a->abc, b->dab, c->efg, d->hcd, e->cda,
 *  f->bef, g->ghc, h->dab starting from "a") .
 * Each run consists of consecutive equal decoded values 1 or 2
 * (letters a,b,e,f become 1; c,d,g,h become 2).  The returned value
 * is the run length divided by 2.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"
#include "../utils/expansion.h"
#include "../utils/to_val.h"

seq_t a(idx_t n)
{
    if (n == 0UL) return (seq_t)0;          /* undefined for n=0 */

    /* ----------------- static state ----------------- */
    static char *word       = NULL;                /* current morphic word */
    static size_t word_len  = 0;                   /* length of word       */
    static size_t processed_len = 0;               /* number of chars processed */

    static seq_t *runs      = NULL;                /* known a(1)…a(k)      */
    static size_t runs_cap  = 0;
    static size_t runs_size = 0;

    static char last_val    = 0;                    /* value of last char   */
    static size_t run_len   = 0;                    /* current unfinished run */

    /* ----------------- initialize ------------------ */
    if (word_len == 0) {
        word = (char *)malloc(1);
        if (!word) return (seq_t)0;                /* out of memory */
        word[0] = 'a';
        word_len = 1;
        processed_len = 0;
    }

    /* ----- generate enough runs for index n ----- */
    while (runs_size < n) {
        /* Expand the current word (3× length) */
        size_t new_len = word_len * 3;
        char *new_word = (char *)malloc(new_len);
        if (!new_word) return (seq_t)0;            /* out of memory */

        size_t p = 0;
        for (size_t i = 0; i < word_len; ++i) {
            const char *exp = expansion(word[i]);
            new_word[p++] = exp[0];
            new_word[p++] = exp[1];
            new_word[p++] = exp[2];
        }

        free(word);
        word = new_word;
        word_len = new_len;

        /* Scan the new suffix, updating run information */
        for (size_t i = processed_len; i < word_len; ++i) {
            char cur_val = to_val(word[i]);

            if (last_val == 0) {          /* first symbol processed */
                last_val = cur_val;
                run_len = 1;
            } else if (cur_val == last_val) {
                ++run_len;
            } else {                      /* run ends */
                /* ensure space for the new run value */
                if (runs_size >= runs_cap) {
                    size_t new_cap = (runs_cap == 0) ? 16 : runs_cap * 2;
                    seq_t *tmp = seq_realloc(runs, runs_cap, new_cap);
                    if (!tmp) return (seq_t)0;  /* allocation failure */
                    runs = tmp;
                    runs_cap = new_cap;
                }
                runs[runs_size++] = (seq_t)(run_len / 2);  /* store value */
                last_val = cur_val;
                run_len = 1;
            }
        }
        processed_len = word_len;
    }

    return runs[n - 1];
}
