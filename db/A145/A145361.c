/*
 * A145361  Characteristic partition array for partitions with parts 1 and 2
 *          only.
 * https://oeis.org/A145361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = 1 if the n-th partition in A–St order has all parts ≤ 2,
 *            otherwise a(n) = 0. The A–St order is obtained by sorting
 *            partitions of each integer first by increasing number of parts,
 *            then within each length by lexicographic order of the parts
 *            (descending). The sequence is the concatenation of...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation lazily builds the flattened array of
    indicators, generating rows of partitions on demand.  Only the
    values 0 and 1 are stored, so the size of seq_t is irrelevant.  */

#include <stddef.h>
#include <stdlib.h>

/* static storage for the flattened indicator sequence */
static seq_t   *flat      = NULL;   /* array of 0/1 indicators */
static size_t   flat_cap  = 0;       /* current allocated capacity */
static size_t   flat_len  = 0;       /* number of filled elements */

/* highest partition number whose row has been generated */
static idx_t   max_row_computed = 0;

/*--- helper to grow the array ---------------------------------------*/
static void ensure_flat_cap(size_t needed)
{
    if (flat_cap >= needed) return;
    size_t new_cap = flat_cap ? (flat_cap << 1) : 8;
    while (new_cap < needed) new_cap <<= 1;
    flat = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
    flat_cap = new_cap;
}

/*--- recursive generation of partitions of 'rem' into exactly
      'partsLeft' parts, each ≤ max_part, all parts non‑increasing.
      all_le2 indicates whether all parts chosen so far are ≤ 2. ---*/
static void gen_exact(idx_t rem, idx_t partsLeft, idx_t max_part, int all_le2)
{
    if (partsLeft == 0) {
        /* a complete partition has been formed */
        if (rem == 0) {
            if (flat_len == flat_cap) ensure_flat_cap(flat_len + 1);
            flat[flat_len++] = (seq_t)(all_le2 ? 1 : 0);
        }
        return;
    }

    /* compute lower and upper bounds for the next part */
    /* lower = ceil(rem / partsLeft) */
    idx_t lower = rem / partsLeft;
    if (rem % partsLeft != 0) ++lower;
    if (lower < 1) lower = 1;

    /* upper = min(rem, max_part, rem - (partsLeft - 1)) */
    idx_t upper = rem;
    if (upper > max_part) upper = max_part;
    idx_t min_remaining = rem - (partsLeft - 1);   /* at least 1 for each of the remaining parts */
    if (upper > min_remaining) upper = min_remaining;

    for (idx_t p = upper; p >= lower; --p) {
        int new_all_le2 = all_le2 & (p <= 2);
        gen_exact(rem - p, partsLeft - 1, p, new_all_le2);
    }
}

/*--- generate one full row -------------------------------------------------*/
static void generate_row(idx_t row)
{
    for (idx_t k = 1; k <= row; ++k) {
        gen_exact(row, k, row, 1);   /* all_le2 = true initially */
    }
    max_row_computed = row;
}

/*--- ensure the flattened array contains at least 'n' elements ---------*/
static void ensure_flat_seq(idx_t n)
{
    if (flat_len >= n) return;
    while (flat_len < n) {
        ++max_row_computed;          /* next row number */
        generate_row(max_row_computed);
    }
}

/*--- public entry point --------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */
    ensure_flat_seq(n);
    return flat[n - 1];
}
