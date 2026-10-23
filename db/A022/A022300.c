/*
 * A022300  The sequence a of 1's and 2's starting with (1,1,2,1) such that
 *          a(n) is the length of the (n+2)nd run of a.
 * https://oeis.org/A022300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9506, u16 n <= 9506, u32 n <= 9506, u64 n <= 9506,
 *            u128 n <= 9506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A022300.
   a(1)=1, a(2)=1, a(3)=2, a(4)=1.
   For n≥5, a(n) equals the length of the (n+2)nd run of a itself;
   runs alternate between values 1 and 2.  Each run length is 1 or 2,
   so the sequence values are always 1 or 2. */

#include <stdlib.h>
#include <stdint.h>

/* Static storage for the generated sequence. */
static seq_t *a_seq   = NULL;   /* array of a(1..) values           */
static size_t a_cap   = 0;      /* current allocated capacity      */
static size_t a_len   = 0;      /* number of elements stored (≥1)   */
static idx_t  run_idx = 1;      /* next run number to generate      */

/* Ensure capacity for at least 'min_cap' elements. */
static void ensure_capacity(idx_t min_cap)
{
    size_t new_cap = a_cap;
    if (new_cap < (size_t)min_cap)
    {
        new_cap = (new_cap == 0) ? 16 : new_cap * 2;
        while (new_cap < (size_t)min_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failed */
        a_seq = tmp;
        a_cap = new_cap;
    }
}

/* Append 'cnt' copies of 'val' to the sequence. */
static void push_value(seq_t val, idx_t cnt)
{
    ensure_capacity(a_len + cnt);
    for (idx_t i = 0; i < cnt; ++i)
        a_seq[a_len++] = val;
}

/* Generate terms of a(n) until at least 'n' terms are available. */
static void ensure_n(idx_t n)
{
    if (n == 0) return;                            /* safety guard */
    while (a_len < (size_t)n)
    {
        seq_t run_value = ((run_idx & 1) ? (seq_t)1 : (seq_t)2);
        idx_t run_length;

        if (run_idx == 1)
            run_length = 2;                         /* first run length */
        else if (run_idx == 2)
            run_length = 1;                         /* second run length */
        else
        {
            /* run k>=3: length = a[k-2], stored at index k-3 (0‑based) */
            seq_t len = a_seq[(size_t)(run_idx - 3)];
            run_length = (len == (seq_t)1) ? (idx_t)1 : (idx_t)2;
        }

        push_value(run_value, run_length);
        ++run_idx;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* undefined for n<1 */
    ensure_n(n);
    return a_seq[(size_t)(n - 1)];                  /* 0‑based indexing */
}
