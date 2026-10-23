/*
 * A131485  Smallest positive integer k == n mod 3 not occurring earlier such
 *          that the sum of two successive terms is a squarefree number.
 * https://oeis.org/A131485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131485: A permutation of the natural numbers.
   a(1)=1. For n>1, a(n) = smallest k>0 such that
       3 | (n - k)   &&   k not used earlier   &&   a(n-1)+k is square‑free.
   The implementation builds the sequence lazily with memoisation. */

/* Note: seq_t and idx_t are defined by the build system. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/is_squarefree.h"

/* Sequence storage ----------------------------------------------------- */
static seq_t *seq      = NULL;   /* array of computed terms, 0‑based. */
static size_t seq_cap  = 0;      /* current capacity of seq. */
static size_t seq_len  = 0;      /* number of terms stored. */

/* Bitmap of used integers --------------------------------------------- */
static bool *used      = NULL;   /* used[i] == true iff i has appeared. */
static size_t used_cap = 0;      /* capacity of used. */

/* Ensure used array can index up to 'need'.  'need' itself is a valid
   index; we allocate the first index > need. */
static void
ensure_used_capacity(idx_t need)
{
    if (used_cap > need) return;
    size_t new_cap = used_cap ? used_cap : 1;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* Ensure seq array can hold at least 'need' elements.  'need' is the
   current number of items the array must support (0‑based). */
static void
ensure_seq_capacity(size_t need)
{
    if (seq_cap > need) return;
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) undefined, return 0 for safety. */

    /* Build the sequence up to n. */
    while (seq_len < (size_t)n) {
        /* Next index to compute (1‑based). */
        idx_t idx = (idx_t)seq_len + 1;

        /* Initial candidate for k: smallest positive integer ≡ n (mod 3). */
        idx_t rem = idx % 3;
        idx_t cand = (rem == 0) ? 3 : rem;

        /* Special case: a(1)=1. */
        if (seq_len == 0) {
            cand = 1;
            ensure_used_capacity(cand);
            used[cand] = true;
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len] = (seq_t)cand;
            seq_len++;
            continue;
        }

        /* Search for the required k. */
        seq_t prev = seq[seq_len - 1];
        while (1) {
            ensure_used_capacity(cand);
            if (!used[cand]) {
                seq_t sum = prev + (seq_t)cand;
                if (is_squarefree(sum)) {
                    used[cand] = true;
                    ensure_seq_capacity(seq_len + 1);
                    seq[seq_len] = (seq_t)cand;
                    seq_len++;
                    break;
                }
            }
            cand += 3;
        }
    }

    return seq[n - 1];
}
