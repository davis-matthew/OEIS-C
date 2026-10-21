/*
 * A075153  Trajectory of 318 under the Reverse and Add! operation carried out
 *          in base 4, written in base 10.
 * https://oeis.org/A075153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0)=318, a(1)=1071, a(n)=a(n-1)+reverse_base4(a(n-1))
 * Range:     u8 none, u16 n <= 7, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where reverse_base4 reverses the base‑4 digits of a number)        */

#include <stdlib.h>

/* reverse the base‑4 representation of v */
static seq_t reverse_base4(seq_t v)
{
    seq_t rev = 0;
    while (v != 0)
    {
        seq_t digit = v & 3;        /* low two bits are the next base‑4 digit */
        rev = (rev << 2) | digit;   /* append digit at the rightmost position   */
        v   = v >> 2;               /* drop the processed digit               */
    }
    return rev;
}

/* resizable cache of computed terms */
static seq_t *cache   = NULL;   /* array of terms */
static size_t cache_size = 0;   /* number of terms stored   */
static size_t cache_cap  = 0;   /* current allocation cap   */

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache with first term */
    if (cache_size == 0)
    {
        ensure_cache_cap(1);
        cache[0] = (seq_t)318;
        cache_size = 1;
    }

    /* compute further terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse_base4(prev);
        seq_t next = prev + rev;
        ensure_cache_cap(i + 1);
        cache[i] = next;
    }

    return cache[(size_t)n];
}
