/*
 * A044763  Numbers n such that string 5,0 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers containing the substring "50" in decimal
   such that the following integer does not contain "50".  This
   generates sequence A044763. */
#include <stdlib.h>
#include <stdbool.h>

/* Helper: check if decimal representation of x contains "50" */
static bool has50(seq_t x)
{
    if (x < (seq_t)10) return false;   /* numbers <10 cannot contain "50" */
    int prev = -1;
    while (x > 0)
    {
        int digit = (int)(x % (seq_t)10);      /* current least‑significant digit */
        if (prev == 0 && digit == 5) return true;  /* pattern 5 (higher), 0 (lower) */
        prev = digit;
        x /= (seq_t)10;
    }
    return false;
}

/* Append a value to the dynamic cache, enlarging it when necessary */
static void append_cache(seq_t val, seq_t **cache, size_t *len, size_t *cap)
{
    if (*len >= *cap)
    {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        *cache = tmp;
        *cap = newcap;
    }
    (*cache)[*len] = val;
    ++(*len);
}

/* Externally linked function required by the framework */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* capacity of the cache */
    static seq_t next_checked = (seq_t)0;   /* last number examined */

    /* Generate terms until we have reached the requested index */
    while (cache_len < (size_t)n)
    {
        seq_t m = next_checked + (seq_t)1;
        seq_t m_next = m + (seq_t)1;
        if (has50(m) && !has50(m_next))
            append_cache(m, &cache, &cache_len, &cache_cap);
        next_checked = m;
    }

    return cache[n - (idx_t)1];
}
