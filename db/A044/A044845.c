/*
 * A044845  Positive integers having no fewer base-4 runs of even length than
 *          odd.
 * https://oeis.org/A044845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044845: Positive integers having no fewer base‑4 runs of even length than odd.
   The routine searches for these integers in increasing order. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if x has at least as many even‑length runs as odd‑length runs
   in its base‑4 representation. */
static bool has_property(seq_t x)
{
    if (x == 0) return false;
    seq_t temp = x;
    int    prev_digit = -1;      /* previous digit seen */
    int    run_len   = 0;        /* length of current run */
    int    even_r    = 0;        /* number of even‑length runs */
    int    odd_r     = 0;        /* number of odd‑length runs */

    while (temp > 0)
    {
        int digit = (int)(temp % 4);   /* digit is always in 0..3 */
        if (prev_digit == -1)          /* first digit */
        {
            prev_digit = digit;
            run_len    = 1;
        }
        else if (digit == prev_digit)  /* same run continues */
        {
            run_len++;
        }
        else                           /* run ended, start new run */
        {
            if ((run_len & 1) == 0) even_r++; else odd_r++;
            prev_digit = digit;
            run_len    = 1;
        }
        temp = temp / 4;
    }
    /* account for the last run */
    if (run_len > 0)
        (run_len & 1) == 0 ? even_r++ : odd_r++;

    return even_r >= odd_r;
}

/* Cache of previously found terms.  cache[i] == a(i+1). */
static seq_t *cache   = NULL;
static size_t cache_size = 0;   /* number of stored terms */
static size_t cache_cap  = 0;   /* capacity of the allocated array */

/* The next integer to examine when generating new terms. */
static seq_t search_val = 1;

/* Ensure that the cache array has at least 'need' elements of capacity. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    /* cast the void* returned by realloc so the code is valid C++13 */
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = p;
    cache_cap = new_cap;
}

/* Generate terms until the cache contains at least 'n' terms. */
static void generate_upto(idx_t n)
{
    while (cache_size < (size_t)n)
    {
        /* Find the next integer that satisfies the property. */
        while (!has_property(search_val)) search_val++;
        ensure_capacity(cache_size + 1);
        cache[cache_size++] = search_val;
        search_val++; /* next number to test */
    }
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1; safe return */
    generate_upto(n);
    return cache[n - 1];
}
