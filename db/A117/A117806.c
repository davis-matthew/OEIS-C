/*
 * A117806  n appears {a(1) + a(2) + ... + a(n-1)} times (with a(1) = 1).
 * https://oeis.org/A117806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A117806:  a(n) is the integer that appears
    sum_{i=1}^{n-1} a(i) times (with a(1) = 1).  */

/*  The function is memoised; it works for any order of calls.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  Supplying types from the build.  */
#ifndef idx_t
/*  idx_t is defined by the build; the line below is only for
    static analysis tools that cannot read the pre‑constructed
    header.  In the real build this line will be omitted.  */
typedef uint64_t idx_t;
#endif

static seq_t *a_arr   = NULL;   /* sequence values, 1‑indexed (a_arr[0] unused) */
static idx_t *cum_arr = NULL;   /* cumulative sums, 1‑indexed (cum_arr[0] = 0) */

/* current state of the generation */
static size_t      seq_cap   = 0;   /* allocated capacity (in elements)   */
static idx_t       seq_len   = 0;   /* number of terms currently stored   */
static idx_t       cur_k     = 2;    /* next integer to output (current block) */
static idx_t       rem       = 0;    /* remaining copies of cur_k in the block */
static bool        init_done = false;

/*  Grow the sequence arrays to hold at least needed elements.  */
static void grow_arrays(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    a_arr   = (seq_t *)realloc(a_arr,
                               (new_cap + 1) * sizeof(seq_t));
    cum_arr = (idx_t *)realloc(cum_arr,
                               (new_cap + 1) * sizeof(idx_t));
    if (!a_arr || !cum_arr) {
        /* insufficient memory – abort (terminate the program).  */
        abort();
    }
    seq_cap = new_cap;
}

/*  Ensure the sequence is computed up to and including index n.  */
static void ensure_a_upto(idx_t n)
{
    if (!init_done) {
        init_done = true;
        /*  initialise the first term.  */
        grow_arrays(1);
        a_arr[1]   = (seq_t)1;
        cum_arr[1] = (idx_t)1;
        seq_len   = 1;
        cur_k     = 2;
        rem       = cum_arr[1];   /* a(2) appears a(1) = 1 time. */
    }

    /*  Often the requested index is already known.  */
    if (n <= seq_len)
        return;

    /*  Extend the sequence until seq_len >= n.  */
    while (seq_len < n) {
        /*  Make room for the next term.  */
        grow_arrays((size_t)(seq_len + 1));

        /*  Append the current integer to the sequence.  */
        ++seq_len;
        a_arr[seq_len]   = (seq_t)cur_k;
        cum_arr[seq_len] = cum_arr[seq_len - 1] + (idx_t)cur_k;

        /*  Consume one occurrence of cur_k.  */
        if (rem > 0)
            --rem;

        /*  If the block of cur_k is finished, move to the next integer.  */
        if (rem == 0) {
            ++cur_k;
            /*  cur_k-1 is guaranteed to be <= seq_len, so cum_arr is defined.  */
            rem = cum_arr[cur_k - 1];
        }
    }
}

seq_t a(idx_t n)
{
    ensure_a_upto(n);
    return a_arr[(size_t)n];
}
