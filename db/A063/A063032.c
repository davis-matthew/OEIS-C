/*
 * A063032  Reversion of y - y^2 - y^3 + y^4 + y^5.
 * https://oeis.org/A063032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A063032 – reversion of y − y² − y³ + y⁴ + y⁵.
   Recurrence (for n ≥ 2):
     a(0) = 0, a(1) = 1
     s₂(n) = Σ_{i=1}^{n−1} a(i)·a(n−i)
     s₃(n) = Σ_{i=1}^{n−1} a(i)·s₂(n−i)
     s₄(n) = Σ_{i=1}^{n−1} a(i)·s₃(n−i)
     s₅(n) = Σ_{i=1}^{n−1} a(i)·s₄(n−i)
     a(n) = s₂(n) + s₃(n) − s₄(n) − s₅(n)
   The arrays s₂…s₅ are maintained incrementally while the sequence
   values are computed.
*/

#include <stdlib.h>

static seq_t *a_cache  = NULL;
static seq_t *s2_cache = NULL;
static seq_t *s3_cache = NULL;
static seq_t *s4_cache = NULL;
static seq_t *s5_cache = NULL;
static size_t  cap     = 0;   /* current capacity of all arrays */
static idx_t   last    = 0;   /* last index computed (≥ 1 when initialised) */

/* Ensure capacity for indices up to need (inclusive). */
static void
ensure_size(idx_t need)
{
    if ((size_t)need + 1 <= cap)
        return;
    size_t new_cap = cap ? cap << 1 : 2;
    while ((size_t)need + 1 > new_cap)
        new_cap <<= 1;

    seq_t *new_a   = (seq_t *)realloc(a_cache,   new_cap * sizeof(seq_t));
    seq_t *new_s2  = (seq_t *)realloc(s2_cache,  new_cap * sizeof(seq_t));
    seq_t *new_s3  = (seq_t *)realloc(s3_cache,  new_cap * sizeof(seq_t));
    seq_t *new_s4  = (seq_t *)realloc(s4_cache,  new_cap * sizeof(seq_t));
    seq_t *new_s5  = (seq_t *)realloc(s5_cache,  new_cap * sizeof(seq_t));
    /* zero the newly allocated portion */
    for (size_t i = cap; i < new_cap; ++i) {
        new_a[i]   = 0;
        new_s2[i]  = 0;
        new_s3[i]  = 0;
        new_s4[i]  = 0;
        new_s5[i]  = 0;
    }
    a_cache   = new_a;
    s2_cache  = new_s2;
    s3_cache  = new_s3;
    s4_cache  = new_s4;
    s5_cache  = new_s5;
    cap       = new_cap;
}

/* Compute values up to n (inclusive). */
static void
compute_up_to(idx_t n)
{
    if (n == 0) return;

    /* one‑time initialisation */
    if (last == 0) {
        ensure_size(1);          /* need indices 0 and 1 */
        a_cache[0] = 0;
        a_cache[1] = 1;
        /* s₂…s₅ at indices 0 and 1 are zero by ensure_size */
        last = 1;
    }

    if (n <= last)
        return;

    ensure_size(n);

    for (idx_t m = last + 1; m <= n; ++m) {
        seq_t s2 = 0, s3 = 0, s4 = 0, s5 = 0;
        for (idx_t i = 1; i < m; ++i) {
            s2 += a_cache[i] * a_cache[m - i];
        }
        s2_cache[m] = s2;
        for (idx_t i = 1; i < m; ++i) {
            s3 += a_cache[i] * s2_cache[m - i];
        }
        s3_cache[m] = s3;
        for (idx_t i = 1; i < m; ++i) {
            s4 += a_cache[i] * s3_cache[m - i];
        }
        s4_cache[m] = s4;
        for (idx_t i = 1; i < m; ++i) {
            s5 += a_cache[i] * s4_cache[m - i];
        }
        s5_cache[m] = s5;

        a_cache[m] = s2 + s3 - s4 - s5;
    }

    last = n;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    compute_up_to(n);
    return a_cache[n];
}
