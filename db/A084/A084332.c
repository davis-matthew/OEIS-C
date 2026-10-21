/*
 * A084332  Duplicate of A081145.
 * https://oeis.org/A084332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth element of the greedy permutation of natural
 * numbers starting with 1 where each successive absolute difference
 * is distinct.  a(0)=1; for n>0, a(n) is the smallest unused positive
 * integer such that |a(n)-a(n-1)| has not appeared before.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state shared among calls */
static seq_t *seq = NULL;           /* array of computed terms          */
static size_t seq_cap = 0;          /* capacity of seq array            */
static size_t seq_len = 0;          /* number of terms stored           */

static bool *used_val = NULL;       /* used_val[x] == true if x already in seq */
static bool *used_absdiff = NULL;   /* used_absdiff[d] == true if |diff| d used */
static size_t used_cap = 0;         /* capacity of the two boolean arrays   */

/* enlarge seq array if needed */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* enlarge both used arrays if needed */
static void ensure_used_cap(size_t needed)
{
    if (needed < used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 8;
    while (new_cap <= needed) new_cap <<= 1;
    bool *new_used_val = (bool *)realloc(used_val, new_cap * sizeof(bool));
    bool *new_used_absdiff = (bool *)realloc(used_absdiff, new_cap * sizeof(bool));
    if (!new_used_val || !new_used_absdiff) exit(EXIT_FAILURE);
    for (size_t i = used_cap; i < new_cap; ++i) {
        new_used_val[i] = false;
        new_used_absdiff[i] = false;
    }
    used_val = new_used_val;
    used_absdiff = new_used_absdiff;
    used_cap = new_cap;
}

/* One-time initialisation of the first term */
static void init_sequence(void)
{
    ensure_seq_cap(1);
    seq[0] = (seq_t)1;
    seq_len = 1;
    ensure_used_cap(1);
    used_val[1] = true;
}

/*
 * Compute a(n).  The function is memoised; once values up to a given
 * index have been computed they are kept for subsequent calls.
 */
seq_t a(idx_t n)
{
    if (seq_len == 0) init_sequence();   /* first entry has already been set */

    while (seq_len <= (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        idx_t cand = 1;
        for (;;) {
            /* make sure we can index into the used arrays */
            ensure_used_cap(cand);

            if (!used_val[cand]) {
                seq_t cand_val = (seq_t)cand;
                seq_t diff = (prev > cand_val) ? (prev - cand_val)
                                               : (cand_val - prev);
                idx_t d = (idx_t)diff;
                ensure_used_cap(d);

                if (!used_absdiff[d]) {
                    /* appropriate candidate found */
                    ensure_seq_cap(seq_len + 1);
                    seq[seq_len]      = cand_val;
                    used_val[cand]    = true;
                    used_absdiff[d]   = true;
                    seq_len++;
                    break;
                }
            }
            ++cand;
        }
    }
    return seq[(size_t)n];
}
