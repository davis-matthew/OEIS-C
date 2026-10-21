/*
 * A240071  Irregular triangle of the simple continued fraction of sqrt(n).
 * https://oeis.org/A240071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9806, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A240071 – Irregular triangle of the simple continued fraction
 *  of sqrt(k).  The n‑th term is obtained by concatenating, for k = 1,2,3,⋯,
 *  the expansion of sqrt(k): first the integer part a0, then all terms
 *  of its periodic part.  The rows are written in order of k and flattened
 *  row‑wise; a(1) is the first element.  For perfect squares only a0 is
 *  stored because the continued fraction has no periodic part.
 */
#include <stdlib.h>
#include <stdint.h>

/* Integer square root: longest integer r such that r*r <= n.  */
static idx_t
isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t r = n >> 1;
    while ((unsigned __int128)r * r > n)
        r >>= 1;
    while ((unsigned __int128)(r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* Dynamic array of computed terms. */
static seq_t *seq   = NULL;   /* sequence buffer      */
static size_t  seq_len = 0;   /* number of terms      */
static size_t  seq_cap = 0;   /* allocated size       */
static idx_t   next_k   = 1;  /* next k for which a
                                row has not yet been generated */

/* Ensure that the buffer has room for at least one more element. */
static void
ensure_capacity(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* insufficient memory – terminate */
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Append a single value to the sequence buffer. */
static inline void
add_term(seq_t v)
{
    ensure_capacity();
    seq[seq_len++] = v;
}

/* Generate the row for a single integer k and append its terms. */
static void
generate_row(idx_t k)
{
    idx_t a0 = isqrt_u64(k);
    add_term((seq_t)a0);

    /* Perfect square: only the integer part is present. */
    if (a0 * a0 == k)
        return;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* Generate the periodic part.  The period ends when a == 2*a0. */
    while (1) {
        idx_t m2 = d * a - m;
        idx_t d2 = (k - m2 * m2) / d;
        idx_t a2 = (a0 + m2) / d2;
        add_term((seq_t)a2);
        if (a2 == 2 * a0)
            break;
        m = m2; d = d2; a = a2;
    }
}

/* Public function that returns the n-th term of the sequence. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* n < 1 is outside the B‑file range */

    /* Keep generating rows until we have at least n terms. */
    while (seq_len < n)
        generate_row(next_k++);

    return seq[n - 1];
}
