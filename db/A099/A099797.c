/*
 * A099797  a(n) is taken to be the smallest positive integer greater than
 *          a(n-1) which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is composite".
 * https://oeis.org/A099797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation of A099797:  
   a(1) = 2 (prime); for n > 1,  a(n) is the smallest integer greater than a(n-1) for which the condition
   "n is in the sequence ⇔ a(n) is composite" holds.  The sequence values are generated
   incrementally and stored in a static dynamic array. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------  Helper: primality test (trial division, sufficient for n ≤ 10 000) --------- */

static bool isComposite(seq_t x)
{
    if (x <= 1)
        return false;           /* 0,1 are neither prime nor composite */
    if (x == 2)
        return false;
    if (x % 2 == 0)              /* even number > 2 is composite */
        return true;
    for (seq_t i = 3; i * i <= x; i += 2)
    {
        if (x % i == 0)
            return true;
    }
    return false;
}

/* ---------  Dynamic array for the sequence  --------- */

static seq_t *seq      = NULL;   /* seq[0] is unused dummy */
static size_t seq_len  = 0;      /* number of stored elements (including dummy) */
static size_t seq_cap  = 0;      /* current capacity */

/* Ensure the array can hold at least n + 1 elements (index 0 … n).  C++ needs a cast. */
static void ensure_capacity(size_t n)
{
    if (seq_cap < n + 1)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = new_seq;
        seq_cap = new_cap;
    }
}

/* ---------  Main function required by the harness  --------- */

seq_t a(idx_t n)
{
    /* Initialize on first call. */
    if (seq_len == 0)
    {
        ensure_capacity(128);
        seq[0]  = 0;          /* dummy so that indices match sequence order */
        seq[1]  = 2;          /* a(1) = 2 (prime) */
        seq_len = 2;          /* seq[1] is the last filled element */
    }

    /* Grow the sequence up to index n.  We can receive calls out of order,
       therefore we compute terms until we have reached n. */
    while ((idx_t)(seq_len - 1) < n)
    {
        /* New index to compute.  seq_len is the count of stored elements
           including dummy, so the next real index is seq_len. */
        idx_t idx = (idx_t)seq_len;

        /* Determine whether idx already appears among earlier a(k). */
        bool in_sequence = false;
        for (idx_t k = 1; k < seq_len; ++k)
        {
            if (seq[k] == (seq_t)idx)
            {
                in_sequence = true;
                break;
            }
        }

        /* Find the smallest candidate larger than the previous term that
           satisfies the rule. */
        seq_t cand = seq[seq_len - 1] + 1;
        while (true)
        {
            bool comp = isComposite(cand);
            if (comp == in_sequence)
                break;
            ++cand;
        }

        /* Store the chosen candidate. */
        ensure_capacity(seq_len);
        seq[seq_len] = cand;
        ++seq_len;
    }

    return seq[n];
}
