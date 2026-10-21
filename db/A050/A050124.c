/*
 * A050124  a(1)=1; for n > 1, a(n) = floor(a(n-1)/5) if this is not among
 *          0,a(1),...,a(n-1); otherwise a(n) = 2*a(n-1).
 * https://oeis.org/A050124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)/5) if not already seen (including 0),
 *            otherwise 2*a(n-1)
 * Range:     u8 n <= 29, u16 n <= 2147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function keeps a memoised table of all computed values.  The
   table is grown as needed.  Membership tests are performed by
   linear scan, which is fast enough for the expected n (≤10000). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment. */
static seq_t *seq = NULL;          /* array of computed terms a(1), a(2), … */
static size_t seq_len = 0;         /* number of terms currently stored */
static size_t seq_cap = 0;         /* current array capacity */

/* Ensure the backing array can hold at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap << 1 : 32;
    while (newcap < need) newcap <<= 1;
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* Test whether value 'x' occurs among the first 'up_to' elements of seq. */
static bool seen_before(size_t up_to, seq_t x)
{
    for (size_t i = 0; i < up_to; ++i)
        if (seq[i] == x) return true;
    return false;
}

/* Compute a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset starts at 1, but guard anyway */

    /* Initialise the memoised sequence on first call. */
    if (seq_len == 0)
    {
        ensure_seq_cap(32);
        seq_len = 1;
        seq[0] = (seq_t)1;             /* a(1) = 1 */
    }

    /* Expand the memoised table up to n. */
    size_t target_len = (size_t)n;
    if (target_len > seq_len)
    {
        ensure_seq_cap(target_len);
        for (size_t idx = seq_len; idx < target_len; ++idx)
        {
            seq_t prev = seq[idx - 1];
            seq_t candidate = prev / (seq_t)5;      /* floor division */

            bool used = (candidate == (seq_t)0);
            if (!used && !seen_before(idx, candidate))
                used = false;
            else
                used = true;

            if (!used)
                seq[idx] = candidate;                /* distinct value */
            else
                seq[idx] = prev * (seq_t)2;           /* repeat, double it */
        }
        seq_len = target_len;
    }

    return seq[n - 1];
}
