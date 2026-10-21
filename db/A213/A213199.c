/*
 * A213199  Numbers k whose Collatz (3x+1) trajectory includes at least one
 *          term that is greater than k.
 * https://oeis.org/A213199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A213199: numbers k whose Collatz (3x+1) trajectory
   ever exceeds k.  The function builds the sequence by testing each
   integer > 2 until it has found n terms.  Results are cached so that
   repeated calls are fast. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static seq_t next_k = 3; /* start checking from 3, a(1) = 3 */

static inline void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = new_arr;
    seq_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    while (seq_len < n)
    {
        seq_t k = next_k;
        seq_t t = k;
        bool exceeds = false;
        /* simulate Collatz trajectory */
        while (t > 1)
        {
            if (t & 1)       /* odd */
                t = t * 3 + 1;
            else             /* even */
                t = t >> 1;   /* t /= 2 */

            if (t > k)
            {
                exceeds = true;
                break;           /* no need to continue */
            }
        }
        if (exceeds)
        {
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len++] = k;
        }
        next_k++;
    }
    return seq_cache[n - 1];
}
