/*
 * A055364  Number of asymmetric mobiles (circular rooted trees) with n nodes
 *          and 3 leaves.
 * https://oeis.org/A055364
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of asymmetric mobiles (circular rooted trees) with
 *            n nodes and 3 leaves. It satisfies the linear recurrence derived
 *            from the generating function a(n) - 3 a(n-1) + 2 a(n-2) + a(n-3)
 *            - a(n-5) - 2 a(n-6) + 3 a(n-7) - a(n-8) = rhs(n), where
 *            rhs(5)=rhs(6)=rhs(8)=1 and rhs(n)=0 otherwise....
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 129, u64 n <= 129,
 *            u128 n <= 129, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_buf = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static void ensure_capacity(size_t need)
{
    if (need < seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need + 1)
        new_cap = need + 1;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!seq_buf)
        abort(); /* cannot allocate, terminate program */
    /* zero any newly allocated slots */
    for (size_t i = seq_cap; i < new_cap; ++i)
        seq_buf[i] = (seq_t)0;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)0; /* undefined for n < 5, any value is acceptable */

    /* Compute values up to index n (inclusive) */
    ensure_capacity((size_t)n);
    for (size_t i = seq_len; i <= (size_t)n; ++i)
    {
        /* RHS contribution */
        seq_t rhs = 0;
        if (i == 5 || i == 6 || i == 8)
            rhs = (seq_t)1;

        /* Isolate a(i) using the recurrence:
           a(i) = rhs + 3 a(i-1) - 2 a(i-2) - a(i-3) + a(i-5) + 2 a(i-6) - 3 a(i-7) + a(i-8) */
        seq_t val = rhs;
        if (i >= 1) val += (seq_t)3 * seq_buf[i - 1];
        if (i >= 2) val += (seq_t)(-2) * seq_buf[i - 2];
        if (i >= 3) val += (seq_t)(-1) * seq_buf[i - 3];
        /* term for i-4 has coefficient 0, can be omitted */
        if (i >= 5) val += (seq_t)1 * seq_buf[i - 5];
        if (i >= 6) val += (seq_t)(2) * seq_buf[i - 6];
        if (i >= 7) val += (seq_t)(-3) * seq_buf[i - 7];
        if (i >= 8) val += (seq_t)1 * seq_buf[i - 8];

        seq_buf[i] = val;
    }
    seq_len = (size_t)n + 1;
    return seq_buf[n];
}
