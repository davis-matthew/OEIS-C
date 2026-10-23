/*
 * A037390  Numbers k such that every base-4 digit of k is a base-8 digit of
 *          k.
 * https://oeis.org/A037390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that every base‑4 digit of k is
 *            also a base‑8 digit of k
 * Range:     u8 n <= 38, u16 n <= 7387, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below builds the sequence lazily.  It keeps a static
 * vector of all found numbers and grows it as needed.
 * The implementation works for the normal unsigned integer types
 * used in the 8, 16, 32, 64 and 128‑bit builds, and for the
 * arbitrary‑precision integer class used in the big build,
 * because only the +, -, *, /, % and comparison operators are used.
 */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* array of found terms          */
static size_t cache_cap = 0;     /* current capacity (in elements)*/
static size_t cache_size = 0;    /* number of terms already found*/
static seq_t last_k = 0;         /* last tested integer           */

/* Resize the cache array to at least 'needed' elements.  The cast
 * is required so that the code compiles as C++ for the big build.
 */
static void grow_cache(size_t needed)
{
    if (needed > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed)
            new_cap *= 2;

        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p)
            abort();           /* allocation failure – stop the program */
        cache = p;
        cache_cap = new_cap;
    }
}

/* Return true iff every digit in the base‑4 representation of k
 * appears somewhere in the base‑8 representation of k.
 * The function uses a 8‑bit bitmask to remember which octal digits
 * have been seen – this works for all integer sizes supported by seq_t.
 */
static bool satisfies(seq_t k)
{
    if (k == 0)
        return false;          /* 0 is not part of the sequence (offset 1) */

    int seen8 = 0;            /* bit i set iff digit i (=0..7) is seen */

    seq_t t = k;
    while (t > 0)
    {
        int d = (int)(t % 8);
        seen8 |= 1 << d;
        t /= 8;
    }

    t = k;
    while (t > 0)
    {
        int d = (int)(t % 4);
        if (!(seen8 & (1 << d)))
            return false;      /* required base‑4 digit not present in base‑8 */
        t /= 4;
    }

    return true;
}

/* Public entry point – returns the n‑th term of the sequence
 * (offset 1).  Calls are allowed in any order.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n=0, return 0 as a fallback */

    while (cache_size < (size_t)n)
    {
        last_k = last_k + 1;    /* test the next integer */

        if (satisfies(last_k))
        {
            grow_cache(cache_size + 1);
            cache[cache_size] = last_k;
            cache_size++;
        }
    }

    return cache[(size_t)n - 1];
}
