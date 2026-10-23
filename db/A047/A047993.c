/*
 * A047993  Number of balanced partitions of n: the largest part equals the
 *          number of parts.
 * https://oeis.org/A047993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, changed
 * Formula:   a(n) = ∑_{k≥1} (-1)^(k-1) * ( p(n-(3k^2-k)/2) - p(n-(3k^2+k)/2)
 *            ) where p(i) is the partition number (p(0)=1, p(i)=0 for i<0)
 * Range:     u8 n <= 28, u16 n <= 63, u32 n <= 164, u64 n <= 489,
 *            u128 n <= 1608, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached partition numbers  p[0..p_max]  (p(0)=1) */
static seq_t *p_array = NULL;
static idx_t   p_max   = 0;

/* Extend the cache so that it contains p[0..N] */
static void ensure_partitions(idx_t N)
{
    if (p_max >= N)
        return;

    /* Allocate / resize the array */
    seq_t *new_arr = (seq_t *)realloc(p_array, (N + 1) * sizeof(seq_t));
    if (!new_arr)
        exit(EXIT_FAILURE);          /* out of memory – abort */
    p_array = new_arr;

    /* Initialise p(0) on the first call */
    if (p_max == 0 && p_array[0] != (seq_t)1)
        p_array[0] = (seq_t)1;

    /* Compute the remaining partition numbers one by one */
    for (idx_t n = p_max + 1; n <= N; ++n) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;   /* (3k^2 - k)/2  */
            idx_t g2 = k * (3 * k + 1) / 2;   /* (3k^2 + k)/2  */
            if (g1 > n && g2 > n)
                break;
            if (g1 <= n) {
                seq_t v = p_array[n - g1];
                if (k & 1)       /* odd k → + */
                    pos += v;
                else             /* even k → – */
                    neg += v;
            }
            if (g2 <= n) {
                seq_t v = p_array[n - g2];
                if (k & 1)      /* odd k → + */
                    pos += v;
                else            /* even k → – */
                    neg += v;
            }
        }
        p_array[n] = pos - neg;
    }
    p_max = N;
}

/* Compute the n-th term of sequence A047993 (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    ensure_partitions(n);        /* make sure p[0..n] are known */

    seq_t pos = 0, neg = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t p1 = k * (3 * k - 1) / 2;   /* (3k^2 - k)/2  */
        idx_t p2 = k * (3 * k + 1) / 2;   /* (3k^2 + k)/2  */
        if (p1 > n && p2 > n)
            break;

        if (p1 <= n) {
            seq_t v = p_array[n - p1];
            if (k & 1)          /* odd k → add */
                pos += v;
            else                /* even k → subtract */
                neg += v;
        }
        if (p2 <= n) {
            seq_t v = p_array[n - p2];
            if (k & 1)          /* odd k → subtract */
                neg += v;
            else                /* even k → add */
                pos += v;
        }
    }
    return pos - neg;
}
