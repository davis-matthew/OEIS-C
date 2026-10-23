/*
 * A044476  Numbers n such that string 3,3 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the base-5 representation of n contains the
 *            substring "33" but the base-5 representation of n+1 does not. An
 *            on‑demand linear search is used, and results are memoised
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if the base‑5 representation of x contains consecutive digits 3,3. */
static bool has33(seq_t x)
{
    int prev = 5;               /* sentinel value not equal to 3 */
    while (x > 0)
    {
        int digit = (int)(x % 5);   /* % with an integer works for big int types */
        if (digit == 3 && prev == 3)
            return true;
        prev = digit;
        x = x / 5;
    }
    return false;
}

/* Cache of already found terms.  The cache is lazily grown. */
static seq_t *term_cache = NULL;   /* array of terms */
static idx_t  cache_cap  = 0;      /* allocated capacity (number of elements) */
static idx_t  cache_len  = 0;      /* number of terms stored */
static seq_t  last_checked = 0;    /* last integer that was examined */

/* Ensure the cache has at least 'need' slots, reallocating if required.
   The C++ warning about void* to seq_t* conversion is avoided with an explicit cast. */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* OEIS entry function. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset 1; harmless return */
        return 0;

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        ++last_checked;        /* examine next integer */
        if (has33(last_checked) && !has33(last_checked + 1))
        {
            ensure_cache_capacity(cache_len + 1);
            term_cache[cache_len++] = last_checked;
        }
    }
    /* Return the (n-1)th stored term (0‑based indexing). */
    return term_cache[n - 1];
}
