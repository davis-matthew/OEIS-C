/*
 * A118607  Start with 1 and repeatedly reverse the digits and add 18 to get
 *          the next term.
 * https://oeis.org/A118607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = reverse_decimal(a(n-1)) + 18, a(1) = 1. The sequence
 *            enters a cycle of length 13: a(n) = a(n-13) for n ≥ 15
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 847 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

/* Cache of computed terms. */
static seq_t *cache   = NULL;
static idx_t   computed = 0;  /* number of terms already computed (>=0) */
static size_t capacity    = 0; /* current allocated capacity */

/* Ensure that the cache can hold `needed` terms. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 4;
    while (new_cap < (size_t)needed)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        exit(1);                    /* allocation failed */
    cache = tmp;
    capacity = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset 1 but harmless */

    if (!cache)                     /* first call – initialise cache */
    {
        capacity = 1;
        cache = mem_new(seq_t, capacity);
        if (!cache)
            exit(1);
        cache[0] = (seq_t)1;       /* a(1) */
        computed = 1;
    }

    if (n <= computed)              /* already known */
        return cache[n - 1];

    ensure_capacity(n);             /* grow cache if necessary */

    for (idx_t i = computed + 1; i <= n; ++i)   /* compute new terms */
    {
        seq_t rev = reverse_digits(cache[i - 2]);      /* a(i-1) is at i-2 */
        cache[i - 1] = rev + (seq_t)18;         /* a(i)  */
    }
    computed = n;                     /* update highest computed index */
    return cache[n - 1];
}
