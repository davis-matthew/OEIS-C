/*
 * A045194  Numbers whose base-5 representation contains exactly one 0 and
 *          three 3's.
 * https://oeis.org/A045194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base-5 representation contains exactly one 0 and
 *            three 3's
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seqs   = NULL;        /* dynamic array of values */
static size_t seq_cap = 0;          /* current capacity of the array */
static idx_t  seq_len = 0;          /* number of values stored */

/* Append a value to the array, growing it if necessary. */
static void append(seq_t val)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *new_ptr = (seq_t *)realloc(seqs, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);

        /* In C++ the pointer type may be a class; the cast keeps the
           assignment simple and the objects are copied by value.  We
           rely on the behaviour that realloc copies bytes, which is
           acceptable for the arbitrary‑precision integer used in the
           big build. */
        seqs = new_ptr;
        seq_cap = new_cap;
    }
    seqs[seq_len++] = val;
}

/* Recursive generation of numbers of a given length L. */
static void rec_generate(idx_t L, idx_t pos,
                         idx_t zeros_used, idx_t threes_used,
                         seq_t cur_val)
{
    if (pos == L) {
        if (zeros_used == 1 && threes_used == 3) {
            append(cur_val);
        }
        return;
    }

    /* Prune branches that cannot possibly meet the required counts. */
    idx_t remaining = L - pos;
    idx_t zeros_needed = 1 - zeros_used;
    idx_t threes_needed = 3 - threes_used;
    if (zeros_needed + threes_needed > remaining) return;

    for (uint8_t d = 0; d < 5; ++d) {
        if (pos == 0 && d == 0) continue;          /* no leading zero */

        idx_t nz = zeros_used + (d == 0 ? 1 : 0);
        idx_t nt = threes_used + (d == 3 ? 1 : 0);
        if (nz > 1 || nt > 3) continue;

        /* Further feasibility check after taking this digit. */
        idx_t new_remaining = remaining - 1;
        idx_t new_zeros_needed = 1 - nz;
        idx_t new_threes_needed = 3 - nt;
        if (new_zeros_needed + new_threes_needed > new_remaining) continue;

        seq_t next_val = cur_val * ((seq_t)5) + (seq_t)d;
        rec_generate(L, pos + 1, nz, nt, next_val);
    }
}

/* Generate all numbers of length L. */
static void generate_len(idx_t L)
{
    rec_generate(L, 0, 0, 0, 0);
}

/* Ensure we have generated the sequence up to at least index n. */
static void ensure_length(idx_t n)
{
    static idx_t max_len_processed = 3;   /* lengths <4 produce no numbers */

    while (seq_len < n) {
        ++max_len_processed;
        generate_len(max_len_processed);
    }
}

/* Public entry point: return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n==0; sentinel */

    ensure_length(n);
    return seqs[n - 1];                    /* zero‑based indexing */
}
