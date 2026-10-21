/*
 * A080199  a(1)=1; for n>1, a(n)=a(n-1) - 3 if n is already in the sequence,
 *          a(n)=a(n-1) + 6 otherwise.
 * https://oeis.org/A080199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A080199
   Recurrence: 
        a(1) = 1;
        For n > 1:
          if integer n has appeared as a term a(k) for some k < n
               a(n) = a(n-1) - 3
          else
               a(n) = a(n-1) + 6
   The implementation memoises the sequence in an array and keeps a
   boolean table that records which values have already occurred, so that
   each membership test is O(1).  The table grows as new values appear.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *seq = NULL;          /* 1‑based sequence array   */
static size_t seq_cap = 0;         /* array capacity           */
static size_t seq_len = 0;         /* last computed index      */
static bool *seen = NULL;          /* boolean table of values  */
static size_t seen_cap = 0;        /* capacity of *seen array  */

/* Ensure *seq can hold at least new_cap elements */
static void grow_seq(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;
    size_t cap = seq_cap ? seq_cap : 128;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* out of memory */
    seq = tmp;
    seq_cap = cap;
}

/* Ensure *seen can index up to new_cap (exclusive) */
static void grow_seen(size_t new_cap)
{
    if (new_cap <= seen_cap)
        return;
    size_t cap = seen_cap ? seen_cap : 128;
    while (cap < new_cap)
        cap <<= 1;
    bool *tmp = (bool *)realloc(seen, cap * sizeof(bool));
    if (!tmp)
        abort();                    /* out of memory */
    /* initialise added elements to false */
    memset(tmp + seen_cap, 0, (cap - seen_cap) * sizeof(bool));
    seen = tmp;
    seen_cap = cap;
}

/* Lazy initialisation on first call */
static void init_static(void)
{
    if (seq != NULL)
        return;                     /* already initialised */

    grow_seq(2);                     /* capacity at least 2 (index 0 unused) */
    seq[1] = (seq_t)1;               /* a(1) = 1                 */
    seq_len = 1;

    grow_seen(2);                    /* allocate *seen                */
    seen[1] = true;                  /* value 1 has appeared        */
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* behaviour below offset arbitrary */

    init_static();

    /* If the requested index has already been computed, return it */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n];

    /* Compute successive terms up to n */
    seq_t cur = seq[seq_len];       /* current a(last)              */

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        /* membership test: does integer i already occur?           */
        bool present = false;
        if ((size_t)i < seen_cap)
            present = seen[(size_t)i];

        if (present)
            cur -= (seq_t)3;
        else
            cur += (seq_t)6;

        grow_seq((size_t)i + 1);            /* ensure space for a(i)  */
        seq[(size_t)i] = cur;
        seq_len = (size_t)i;

        /* Mark the newly produced value as seen */
        size_t val = (size_t)cur;
        grow_seen(val + 1);
        seen[val] = true;
    }

    return seq[(size_t)n];
}
