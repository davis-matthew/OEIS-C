/*
 * A225743  Triangular array: row n is least squarefree word of length n using
 *          positive integers.
 * https://oeis.org/A225743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A225743:
   The sequence is obtained by flattening, row-wise, the least
   square‑free word of length n over positive integers.
   The word is generated greedily: at each position the smallest
   positive integer that does not create a square in the prefix
   is chosen.  The function returns the n‑th term (1‑based) of
   this flattened array. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Static storage for the generated word. */
static seq_t *word = NULL;
static size_t word_cap = 0;   /* allocated capacity */
static size_t word_len = 0;   /* number of symbols produced */

/* Ensure that the word buffer can hold at least *cap* symbols. */
static void ensure_capacity(size_t cap)
{
    if (cap <= word_cap)
        return;
    size_t newcap = word_cap ? word_cap : 1;
    while (newcap < cap)
        newcap <<= 1;
    word = (seq_t*)realloc(word, newcap * sizeof(*word));
    word_cap = newcap;
}

/* Test whether appending *val* at position *pos* keeps the word squarefree. */
static bool is_squarefree(const seq_t *w, idx_t pos, seq_t val)
{
    idx_t maxl = (pos + 1) / 2;
    for (idx_t l = 1; l <= maxl; ++l)
    {
        bool equal = true;
        for (idx_t j = 0; j < l; ++j)
        {
            seq_t left  = w[(size_t)(pos - 2 * l + 1 + j)];
            seq_t right = (j == l - 1) ? val : w[(size_t)(pos - l + 1 + j)];
            if (left != right)
            {
                equal = false;
                break;
            }
        }
        if (equal)   /* a square would be created */
            return false;
    }
    return true;
}

/* Extend the word array so that it contains at least *len* symbols. */
static void generate_word(idx_t len)
{
    if (len <= word_len)
        return;
    ensure_capacity((size_t)len);
    for (idx_t pos = word_len; pos < len; ++pos)
    {
        seq_t val = 1;
        while (true)
        {
            if (is_squarefree(word, pos, val))
            {
                word[(size_t)pos] = val;
                break;
            }
            ++val;
        }
    }
    word_len = (size_t)len;
}

/* Find the smallest r with r*(r+1)/2 ≥ n. */
static idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while (((unsigned __int128)hi * (hi + 1)) / 2 < n)
        hi <<= 1;
    while (lo + 1 < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n)
            hi = mid;
        else
            lo = mid;
    }
    return hi;
}

/* Return the n‑th term of A225743 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; undefined for 0 */
        return 0;
    idx_t row = find_row(n);          /* row containing position n */
    generate_word(row);               /* ensure word of length row exists */
    idx_t start = ((unsigned __int128)(row - 1) * row) / 2;  /* first index of row (0‑based) */
    idx_t offset = n - start - 1;          /* 0‑based position within the row */
    return word[(size_t)offset];
}
