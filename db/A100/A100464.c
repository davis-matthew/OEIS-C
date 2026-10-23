/*
 * A100464  Write down the numbers from 3 to infinity. Take next number, M
 *          say, that has not been crossed off and cross off all the numbers
 *          i*M + 1 for i >= 1. Repeat. The numbers that are left form the
 *          sequence.
 * https://oeis.org/A100464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each candidate number starting from 3, if n-1 is not divisible
   by any previously chosen sequence element, then n is the next term.
   The function builds the sequence lazily and returns the n‑th value.
   It works for any unsigned integer types seq_t and idx_t (e.g. uint64_t, 
   unsigned __int128, or an arbitrary‑precision integer class). */

#include <stdbool.h>
#include <stdlib.h>

/* static storage for the sequence (1‑based index in seq[0 … len-1]) */
static idx_t *seq = NULL;
static size_t seq_cap = 0; /* allocated capacity */
static size_t seq_len = 0; /* number of elements stored */

/* helper: enlarge seq array if needed (C++ casts required) */
static void ensure_seq_capacity(size_t minsize)
{
    if (minsize <= seq_cap)
        return;
    size_t newcap = seq_cap ? seq_cap * 2 : 64;
    if (newcap < minsize)
        newcap = minsize;
    idx_t *tmp = (idx_t *)realloc(seq, newcap * sizeof(idx_t));
    if (tmp == NULL)
        abort();                       /* allocation failed */
    seq = tmp;
    seq_cap = newcap;
}

/* the generated sequence values are stored in static state */
static idx_t current_candidate = 3; /* next number to test */

/* Return the n‑th term of the sequence (1-based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n=0, return zero */

    while (seq_len < (size_t)n) {
        /* Test whether current_candidate survives */
        bool crossed = false;
        for (size_t i = 0; i < seq_len && !crossed; ++i) {
            if ((current_candidate - 1) % seq[i] == 0)
                crossed = true;
        }

        if (!crossed) {
            /* new term found */
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = current_candidate;
        }

        ++current_candidate;
    }

    return (seq_t)seq[n - 1];
}
