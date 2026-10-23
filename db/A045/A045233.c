/*
 * A045233  Numbers whose base-5 representation contains no 1's and exactly
 *          one 3.
 * https://oeis.org/A045233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Enumerate all base‑5 numbers that use only the digits {0,2,3,4},
 *            contain exactly one digit '3', and no digit '1'. The numbers are
 *            produced in increasing numeric order, which coincides with
 *            increasing base‑5 length and lexicographic order of the digits
 * Range:     u8 n <= 28, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache      = NULL;    /* stored sequence values */
static size_t cache_cap  = 0;        /* current capacity of cache */
static size_t cache_sz   = 0;        /* number of values actually stored */
static idx_t  max_len_done = 0;      /* largest base‑5 length already generated */

/* Add a generated number to the cache, resizing as needed. */
static void append_to_cache(seq_t val)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

/* Recursive generator for a fixed base‑5 length.  ‘pos’ is the
   current position (0 = most significant), ‘len’ is the total
   length, 'val' is the value built so far, and ‘used3’ indicates
   whether the digit 3 has already been used. */
static void rec(idx_t pos, idx_t len, seq_t val, bool used3)
{
    /* Available digits in ascending order */
    const unsigned digits[4] = {0, 2, 3, 4};
    for (int i = 0; i < 4; ++i) {
        unsigned d = digits[i];

        /* Reject leading zero and a second '3' */
        if (pos == 0 && d == 0)          continue;
        if (d == 3 && used3)             continue;

        bool new_used = used3 || (d == 3);
        seq_t new_val = val * (seq_t)5 + (seq_t)d;

        if (pos == len - 1) {            /* last digit */
            if (new_used)                /* exactly one 3 used */
                append_to_cache(new_val);
        } else {
            rec(pos + 1, len, new_val, new_used);
        }
    }
}

/* Generate all numbers of a given base‑5 length. */
static void generate_len(idx_t len)
{
    rec(0, len, (seq_t)0, false);
}

/* Ensure that the cache contains at least ‘n’ terms. */
static void generate_up_to(idx_t n)
{
    while (cache_sz < (size_t)n) {
        ++max_len_done;
        generate_len(max_len_done);
    }
}

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* unused; offset is 1 */

    generate_up_to(n);                      /* compute if necessary */
    return cache[n - 1];
}
