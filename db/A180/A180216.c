/*
 * A180216  Base 10 representation of numbers which, in base 2, satisfy
 *          Abs(k-Reverse(k))+Reverse(Abs(k-Reverse(k)))=k.
 * https://oeis.org/A180216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based generation of A180216 */
/* A(n) = the base‑10 representation of the n-th integer k (0‑indexed) such that
   in base 2, |k – rev(k)| + rev(|k – rev(k)|) = k, where rev(x) reverses the
   binary digits of x. This is implemented by brute‑force enumeration,
   caching the found values. */

#include <stdlib.h>
#include <stdbool.h>

/* Reverse the bits of x, ignoring leading zeros. */
static seq_t reverse_bits(seq_t x)
{
    seq_t rev = 0;
    while (x)
    {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

/* Test whether k satisfies the defining equation. */
static bool is_good(seq_t k)
{
    seq_t rev   = reverse_bits(k);
    seq_t diff  = (k > rev) ? k - rev : rev - k;
    seq_t revdiff = reverse_bits(diff);
    return (revdiff + diff) == k;
}

/* Ensure the dynamic array has at least 'needed' slots. */
static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed)
    {
        size_t newcap = (*cap == 0) ? 4 : *cap * 2;
        while (newcap < needed) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = newcap;
    }
}

/* Return the n-th element (0‑indexed) of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached found values */
    static size_t  cap   = 0;     /* allocated capacity of cache */
    static size_t  len   = 0;     /* number of stored values */
    static seq_t  tested = 0;     /* next integer to test */

    while (len <= n)
    {
        if (is_good(tested))
        {
            ensure_cap(&cache, &cap, len + 1);
            cache[len++] = tested;
        }
        ++tested;
    }
    return cache[n];
}
