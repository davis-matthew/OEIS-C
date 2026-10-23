/*
 * A043895  Numbers k such that 3 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) returns the nth positive integer k such that
   the base‑5 representation of k contains adjacent digits 3 and 4
   in either order (34 or 43) and the base‑5 representation of k+1
   does not contain such a pair.  The function builds these terms
   on demand by scanning forward from the last found value. */
#include <stdbool.h>
#include <stdlib.h>

/* Test whether x (in decimal) has adjacent digits 3 and 4 in base‑5. */
static bool has34(seq_t x)
{
    /* Numbers less than 15 cannot have two adjacent digits 3 and 4. */
    if (x < 15)
        return false;

    seq_t prev = 5;          /* sentinel value (not a digit 0‑4) */
    while (x > 0)
    {
        seq_t d = x % 5;
        x /= 5;
        /* Detect 34 or 43 adjacency. */
        if (d == 3 && prev == 4)
            return true;
        if (d == 4 && prev == 3)
            return true;
        prev = d;
    }
    return false;
}

/* Return the n-th term (offset 1) of sequence A043895. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n=0, safe return */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (cache[0] = a(1)) */
    static idx_t cache_cap = 0;
    static seq_t last_k = 0;      /* last k that satisfied the condition */

    /* Allocate or grow the cache if needed. */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;          /* allocation failure */
    }
    else if (n > cache_cap)
    {
        idx_t new_cap = cache_cap;
        while (n > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if it already exists. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Search for further terms up to n. */
    seq_t k = last_k + 1;
    while (cache_len < n)
    {
        if (has34(k) && !has34(k + 1))
        {
            cache[cache_len++] = k;
            last_k = k;
        }
        ++k;
    }

    return cache[n - 1];
}
