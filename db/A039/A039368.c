/*
 * A039368  Numbers whose base-9 representation has the same nonzero number of
 *          1's and 5's.
 * https://oeis.org/A039368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 8513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9727 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th integer whose base‑9 representation
   contains the same non‑zero number of 1's and 5's. Offset = 1. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* true iff x has the same non‑zero number of 1’s and 5’s in base 9 */
static bool has_equal_ones_fives(seq_t x)
{
    unsigned cnt1 = 0, cnt5 = 0;
    seq_t tmp = x;
    while (tmp != 0)
    {
        unsigned d = (unsigned)(tmp % 9);
        if (d == 1)         cnt1++;
        else if (d == 5)    cnt5++;
        tmp = tmp / 9;
    }
    return cnt1 > 0 && cnt1 == cnt5;
}

static seq_t *cache = NULL;      /* found terms */
static size_t cache_len = 0;     /* number stored so far */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t next_val = 0;       /* next integer to test */

/* Advance next_val to the next integer satisfying the condition.
   Return false if the value overflows the underlying seq_t type. */
static bool advance_to_next_valid(void)
{
    seq_t cur = next_val;
    while (!has_equal_ones_fives(cur))
    {
        seq_t nxt = cur + 1;
        /* If nxt is less than cur, an unsigned overflow has occurred. */
        if (nxt < cur)
            return false;
        cur = nxt;
    }
    next_val = cur;
    return true;
}

/* Ensure that cache_len ≥ n.  Scan integers until n terms are found. */
static void extend_cache(idx_t n)
{
    size_t needed = (size_t)n;
    while (cache_len < needed)
    {
        if (!advance_to_next_valid())
            break;  /* cannot find more within the seq_t range */

        /* grow the array if needed */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            if (new_cap < cache_len + 1)
                new_cap = cache_len + 1;
            grow_seq_array(&cache, &cache_cap, new_cap);
        }

        cache[cache_len++] = next_val;
        /* prepare for searching the following candidate */
        ++next_val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* behavior below the offset is unspecified */
    extend_cache(n);
    return cache[n - 1];          /* a(1) is stored at cache[0] */
}
