/*
 * A080646  a(1) = 3; for n>1, a(n) is taken to be the smallest integer
 *          greater than a(n-1) which is consistent with the condition "if n
 *          is a member of the sequence then a(n) is divisible by 4".
 * https://oeis.org/A080646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A080646:  a(1)=3; for n>1, a(n) is the smallest integer > a(n-1)
 *  such that if n already appears as a term of the sequence then a(n)
 *  is divisible by 4.  The function memoises previous terms and
 *  recomputes only up to the requested index.  It works for any
 *  SEQ_T/IDX_T combination that supports integer arithmetic. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static idx_t seq_len_up_to = 0;        /* maximum n computed previously   */
static seq_t *seq_vals      = NULL;    /* stored a(n) values (index starts at 1) */
static size_t seq_cap       = 0;       /* allocated capacity for seq_vals   */

static unsigned char *seen_val  = NULL;/* bit array of values that have appeared */
static size_t seen_cap          = 0;   /* capacity for seen_val (value+1)    */

/* Ensure seq_vals has space for indices up to max_n (inclusive).           */
static void ensure_seq_cap(idx_t max_n)
{
    if ((size_t)max_n + 1 <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap <= (size_t)max_n + 1) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_vals = tmp;
    seq_cap = new_cap;
}

/* Ensure seen_val can index up to value (value+1).                         */
static void ensure_seen_cap(seq_t value)
{
    if ((size_t)value + 1 <= seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap * 2 : 256;
    while (new_cap <= (size_t)value + 1) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(seen_val, new_cap);
    /* zero the new region */
    memset(tmp + seen_cap, 0, new_cap - seen_cap);
    seen_val = tmp;
    seen_cap = new_cap;
}

/* Compute sequence values up to index n.  idempotent.                       */
static void compute_up_to(idx_t n)
{
    if (seq_len_up_to >= n) return;          /* already computed   */

    ensure_seq_cap(n);

    for (idx_t i = seq_len_up_to + 1; i <= n; ++i)
    {
        if (i == 1)
        {
            seq_vals[i] = (seq_t)3;
        }
        else
        {
            seq_t candidate = seq_vals[i - 1] + (seq_t)1;
            /* If i has already appeared as a term, require divisibility by 4 */
            if (i < seen_cap && seen_val[(size_t)i])
            {
                seq_t mod = candidate & (seq_t)3;   /* candidate % 4 */
                if (mod != (seq_t)0)
                    candidate += (seq_t)(4 - mod);
            }
            seq_vals[i] = candidate;
        }

        /* Record that this value has appeared */
        ensure_seen_cap(seq_vals[i]);
        seen_val[(size_t)seq_vals[i]] = 1;
    }
    seq_len_up_to = n;
}

/* Public API ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */
    compute_up_to(n);
    return seq_vals[n];
}
