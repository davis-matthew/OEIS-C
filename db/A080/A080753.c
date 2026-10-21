/*
 * A080753  a(1)=2; for n > 1, a is the second-smallest positive integer
 *          greater than a(n-1) such that the condition "n is in the sequence
 *          if and only if a(n) is odd" is satisfied.
 * https://oeis.org/A080753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(1) = 2;
 * for n > 1, let S = {a(k) : k ≥ 1}.  
 * a(n) is the second‑smallest integer > a(n‑1) such that
 *      n ∈ S   ⇔   a(n) is odd.
 *
 * The implementation builds the sequence incrementally,
 * storing a(n) and the presence information in static arrays.
 * It runs in O(n) time for successive queries.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* static storage used by the function */
static seq_t *seq        = NULL;  /* array of a(k) */
static size_t seq_cap    = 0;
static size_t seq_len    = 0;

static bool *present     = NULL;  /* present[v] == true iff v ∈ S */
static size_t present_cap = 0;

/* enlarge the seq array to hold at least ‘need’ elements */
static void ensure_seq(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* enlarge the present array to hold indices up to ‘need‑1’ */
static void ensure_present(size_t need)
{
    if (need <= present_cap) return;
    size_t new_cap = present_cap ? present_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    present = (bool *)realloc(present, new_cap * sizeof(bool));
    for (size_t i = present_cap; i < new_cap; ++i) present[i] = false;
    present_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise first term if required */
    if (seq_len == 0) {
        ensure_seq(1);
        seq[0] = 2;
        seq_len = 1;
        /* we need indices 0 … 2 for present[2] */
        ensure_present(3);
        present[2] = true;          /* 2 ∈ S after the first term */
    }

    /* we need to look at present[n] before using it */
    if ((size_t)n + 1 > present_cap)
        ensure_present((size_t)n + 1);

    /* already computed? */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* generate terms up to n */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
        seq_t prev = seq[seq_len - 1];
        seq_t cand = prev + 1;

        /* target parity: 1 iff n has already appeared as a term */
        unsigned int target = present[i] ? 1U : 0U;

        seq_t val;
        if ((cand & 1U) == target)
            val = cand + 2;   /* second such integer */
        else
            val = cand + 3;   /* first mismatched, then +2 */

        /* record the new value */
        ensure_present((size_t)val + 1);
        present[(size_t)val] = true;

        ensure_seq(seq_len + 1);
        seq[seq_len++] = val;
    }

    return seq[(size_t)n - 1];
}
