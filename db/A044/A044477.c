/*
 * A044477  Numbers n such that string 3,4 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the string "34" occurs in the base‑5
   representation of n but not of n+1.
   The function generates terms on demand by scanning successive integers
   and memoises already‑found terms.
   No special formula is known, so a brute‑force search is used. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if x (in base 5) contains the substring "34". */
static bool has_34(seq_t x)
{
    seq_t v = x;
    int prev = -1;          /* previous least‑significant digit seen */
    int curr;

    /* Use a do‑while so that x = 0 is handled correctly (it yields false). */
    do {
        curr = (int)(v % 5);
        if (prev == 4 && curr == 3)
            return true;
        prev = curr;
        v /= 5;
    } while (v > 0);

    return false;
}

/* Memoised storage of generated terms. */
static seq_t *cache   = NULL;   /* array of found terms           */
static size_t cache_len = 0;    /* number of stored terms         */
static size_t cache_cap = 0;    /* current allocation capacity    */
static seq_t candidate   = 0;   /* next integer to test */

static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < need)
        new_cap = need;

    /* realloc works for trivial types; the cast is required for portable
       C++. For non‑trivial seq_t types the programs that use this function
       are expected to provide a trivial default constructor. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* out of memory */

    cache = tmp;
    cache_cap = new_cap;
}

static void add_term(seq_t val)
{
    grow_cache(cache_len + 1);
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n < 1 – safe default */

    /* Ensure we have at least n terms stored. */
    while (cache_len < (size_t)n) {
        if (has_34(candidate) && !has_34(candidate + 1))
            add_term(candidate);
        candidate++;
    }

    return cache[n - 1];
}
