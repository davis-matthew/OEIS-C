/*
 * A102528  a(n)=least positive integer not a(k) or a(k)+floor(k/2) for any
 *          k<n.
 * https://oeis.org/A102528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = least positive integer not equal to a(k) or a(k)+floor(k/2)
   for any k < n.  Offset 1 (a(1)=1).  This implementation builds the
   sequence iteratively, keeping an array of the terms and a Boolean
   table of forbidden numbers.
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Dynamically‑allocated arrays for the sequence and the used table. */
static seq_t *seq = NULL;           /* seq[0] = a(1), seq[1] = a(2), …            */
static idx_t seq_length = 0;       /* number of terms currently stored          */
static idx_t seq_capacity = 0;     /* allocated size of seq[]                   */

static bool *used = NULL;          /* used[v] == true  ⇔  v is forbidden       */
static idx_t used_capacity = 0;    /* allocated size of used[]                  */

static idx_t next_candidate = 1;   /* smallest positive integer not yet used   */

/* Ensure seq has space for at least 'min_cap' elements. */
static void ensure_seq_capacity(idx_t min_cap)
{
    if (seq_capacity >= min_cap)
        return;
    idx_t new_cap = seq_capacity ? seq_capacity * 2 : 8;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    seq = tmp;
    seq_capacity = new_cap;
}

/* Ensure used has indices up to 'max_idx' inclusive. */
static void ensure_used_capacity(idx_t max_idx)
{
    if (used_capacity > max_idx)
        return;
    idx_t new_cap = used_capacity ? used_capacity * 2 : 64;
    while (new_cap <= max_idx)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp)
        exit(EXIT_FAILURE);
    /* Initialize newly allocated entries to false. */
    if (used) {
        size_t diff = new_cap - used_capacity;
        memset(tmp + used_capacity, 0, diff * sizeof(bool));
    } else {
        memset(tmp, 0, new_cap * sizeof(bool));
    }
    used = tmp;
    used_capacity = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n=0, but return 0 */

    /* Generate terms up to the requested index. */
    while (seq_length < n) {
        /* Find the smallest positive integer not yet marked as used. */
        while (next_candidate < used_capacity && used[next_candidate])
            ++next_candidate;
        if (next_candidate >= used_capacity)
            ensure_used_capacity(next_candidate);

        /* Append the new term. */
        ensure_seq_capacity(seq_length + 1);
        seq[seq_length] = (seq_t)next_candidate;
        seq_length++;

        /* Mark the term itself as forbidden. */
        used[(idx_t)next_candidate] = true;

        /* Compute a(k)+floor(k/2) for this new term (k = seq_length). */
        idx_t k = seq_length;                      /* 1‑based index of new term   */
        seq_t val2 = seq[seq_length - 1] + (seq_t)(k / 2); /* b(k) */

        idx_t idx_val2 = (idx_t)val2;
        if (idx_val2 >= used_capacity)
            ensure_used_capacity(idx_val2);
        used[idx_val2] = true;

        /* Advance the candidate for the next iteration. */
        ++next_candidate;
    }

    return seq[n - 1];
}
