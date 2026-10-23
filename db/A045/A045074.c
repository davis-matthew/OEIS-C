/*
 * A045074  Numbers whose base-4 representation contains exactly two 0's and
 *          three 3's.
 * https://oeis.org/A045074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base-4 representation contains exactly two
 *            0's and three 3's
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 */

#include <stdlib.h>
#include <stdint.h>

/* Cached sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;  /* number of stored terms   */
static size_t seq_cap  = 0;  /* current capacity of cache */
static size_t current_length = 5; /* length of numbers to generate next */

/* Append a new value to the cache, expanding it if necessary */
static void push_val(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 64;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* Recursive DFS that builds base‑4 numbers of a fixed length.
   - pos         : current position (0 … L-1)
   - L           : total length of the number
   - zeros_left  : how many 0's still need to be placed
   - threes_left : how many 3's still need to be placed
   - val         : current value formed by the already fixed digits
*/
static void dfs_gen(size_t pos, size_t L,
                    size_t zeros_left, size_t threes_left,
                    seq_t val)
{
    if (pos == L) { /* all positions fixed */
        if (zeros_left == 0 && threes_left == 0)
            push_val(val);
        return;
    }

    size_t remaining = L - pos; /* positions still to fill, including current */

    for (seq_t d = 0; d <= 3; ++d) {
        /* Leading digit cannot be zero */
        if (pos == 0 && d == 0)
            continue;

        /* Enforce the required counts */
        if (d == 0 && zeros_left == 0)
            continue;
        if (d == 3 && threes_left == 0)
            continue;

        size_t nz = zeros_left - (d == 0);
        size_t nt = threes_left - (d == 3);

        /* After placing this digit, the remaining places must fit the remaining
           zeros and threes */
        if (nz > remaining - 1 || nt > remaining - 1 ||
            (nz + nt) > remaining - 1)
            continue;

        dfs_gen(pos + 1, L, nz, nt, val * 4 + d);
    }
}

/* Generate all numbers of the current_length and append them to the cache.
   Afterwards current_length is incremented. */
static void generate_current_length(void)
{
    dfs_gen(0, current_length, 2, 3, 0);
    ++current_length;
}

/* Ensure that at least n terms are available in the cache. */
static void ensure_terms(idx_t n)
{
    if (n <= seq_len)
        return;

    while (seq_len < (size_t)n)
        generate_current_length();
}

/* Main function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* under‑offset behaviour is undefined */
    ensure_terms(n);
    return seq_cache[n - 1];
}
