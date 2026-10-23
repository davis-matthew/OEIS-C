/*
 * A168315  Triangle read by rows, A168313 * the diagonalized variant of its
 *          eigensequence, A168314.
 * https://oeis.org/A168315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The triangle T is defined by T[1,1] = 1 For n ≥ 2 let half =
 *            ⌊n/2⌋. • The first half elements of row n are 0. • For k =
 *            half+1,…,n-2 (if any) T[n,k] = T[n-1,k]. • T[n,n-1] = 2 ·
 *            T[n-1,n-1] (only if n-1 > half) • T[n,n] = sum of the previous
 *            row. The sequence a(n) is the triangle read row‑by‑row (...
 * Range:     u8 n <= 53, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL;  /* flattened sequence cache  */
static size_t cache_cap  = 0;     /* allocated slots in cache   */
static size_t cache_len  = 0;     /* number of terms cached     */

static seq_t *prev_row   = NULL;  /* last generated row         */
static size_t prev_len   = 0;     /* length of prev_row         */
static seq_t prev_sum    = 0;     /* sum of prev_row            */

/* Append a value to the cached sequence, growing the array if needed. */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Initialise the first row (row 1).  This is run only once. */
static void init_first_row(void)
{
    prev_len  = 1;
    prev_row  = (seq_t *)malloc(prev_len * sizeof(seq_t));
    prev_row[0] = (seq_t)1;
    prev_sum = (seq_t)1;
    cache_push(prev_row[0]);                     /* a(1) */
}

/* Generate rows until the requested index n is reached. */
static void generate_up_to(idx_t n)
{
    if (cache_len == 0)
        init_first_row();

    while (cache_len < (size_t)n)
    {
        /* Generate next row: row number = prev_len + 1    */
        size_t new_len = prev_len + 1;
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));

        size_t half = new_len / 2;              /* floor(n/2) */
        seq_t new_sum = (seq_t)0;

        for (size_t i = 0; i < new_len; ++i)
        {
            if (i == new_len - 1)               /* last element */
            {
                new_row[i] = prev_sum;
                new_sum += new_row[i];
                cache_push(new_row[i]);
            }
            else if (i == new_len - 2 && i >= half) /* second‑last element */
            {
                new_row[i] = prev_row[prev_len - 1] * (seq_t)2;
                new_sum += new_row[i];
                cache_push(new_row[i]);
            }
            else if (i < half)                  /* leading zeros */
            {
                new_row[i] = (seq_t)0;
                new_sum += new_row[i];
                cache_push(new_row[i]);
            }
            else                                 /* middle part */
            {
                new_row[i] = prev_row[i];
                new_sum += new_row[i];
                cache_push(new_row[i]);
            }
        }

        /* Advance to next row */
        free(prev_row);
        prev_row = new_row;
        prev_len = new_len;
        prev_sum = new_sum;
    }
}

/* External entry point required by the build system. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                 /* offset is 1; n==0 unused */
    generate_up_to(n);
    return cache[n - 1];
}
