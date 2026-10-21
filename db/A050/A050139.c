/*
 * A050139  a(1)=2; for n > 1, a(n) = floor(a(n-1)/2) if this is not among 0,
 *          a(1), ..., a(n-1); otherwise a(n) = 4*n.
 * https://oeis.org/A050139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>1, a(n)=floor(a(n-1)/2) if this value is not in
 *            {0, a(1), ..., a(n-1)}; otherwise a(n)=4*n
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *seq_arr      = NULL; /* array of computed a(n) values, 1-based */
static size_t seq_cap      = 0;    /* allocated capacity of seq_arr */
static idx_t  seq_last     = 0;    /* highest index computed so far */

static bool *seen          = NULL; /* bitmask of values that have appeared */
static size_t seen_cap     = 0;    /* allocated capacity of seen[] */

static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap == 0 ? 2 : seq_cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_seen_cap(size_t needed)
{
    if (seen_cap >= needed) return;
    size_t new_cap = seen_cap == 0 ? 8 : seen_cap;
    while (new_cap < needed) new_cap <<= 1;
    bool *new_seen = (bool *)realloc(seen, new_cap * sizeof(bool));
    for (size_t i = seen_cap; i < new_cap; ++i) new_seen[i] = false;
    seen = new_seen;
    seen_cap = new_cap;
}

static void init_sequence(void)
{
    /* Start with a(1)=2 */
    ensure_seq_cap(3);               /* need indices 0..2 */
    seq_arr[0] = 0;                  /* unused placeholder */
    seq_arr[1] = 2;
    seq_last = 1;

    ensure_seen_cap(4 * 1 + 10);      /* accommodate at least value 2 */
    seen[0] = true;                  /* 0 is considered already used */
    seen[2] = true;                  /* a(1) */
}

/* Computes a(n) with memoisation.  Works for arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, but safe guard */

    if (!seq_arr) init_sequence();

    /* Ensure we can store all values up to 4*n */
    size_t max_val_needed = 4 * (size_t)n + 10;
    ensure_seen_cap(max_val_needed);

    /* Extend sequence up to n if necessary */
    for (idx_t i = seq_last + 1; i <= n; ++i)
    {
        seq_t prev = seq_arr[i - 1];
        seq_t candidate = prev >> 1;                     /* floor(prev/2) */
        bool conflict = false;

        if (candidate == 0)
            conflict = true;
        else if (candidate < seen_cap && seen[(size_t)candidate])
            conflict = true;

        seq_t value;
        if (conflict)
            value = (seq_t)(4 * i);
        else
            value = candidate;

        ensure_seq_cap((size_t)i + 1);
        seq_arr[i] = value;
        seq_last = i;

        if ((size_t)value >= seen_cap)
            ensure_seen_cap((size_t)value + 10);
        seen[(size_t)value] = true;
    }

    return seq_arr[n];
}
