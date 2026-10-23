/*
 * A035253  Second differences are
 *          2,2,1,2,1,1,2,1,1,1,2,1,1,1,1,2,1,1,1,1,1,2,.. (A035214).
 * https://oeis.org/A035253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A060432(n-1)+4+n*(n-7)/2
 *   This is computed using the recurrence derived from the given
 *   second-difference pattern: 2 occurs at indices n = 2 + k(k+1)/2,
 *   otherwise the second difference is 1.
 *
 *   a(0) = 4, a(1) = 1
 */
#include <stdint.h>

/* Return the n-th term of OEIS sequence A035253 */
seq_t a(idx_t n)
{
    /* Handle the first few terms explicitly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)1;

    /* Signed 64‑bit intermediates are safe for n up to many thousands */
    int64_t diff  = -3;      /* d[1] = a[1] - a[0] */
    int64_t a_prev = 1;      /* a[1] */
    int64_t a_curr = 0;      /* placeholder for a[n] */

    uint64_t next_two = 2;   /* first index where second diff = 2 */
    uint64_t gap = 1;        /* increment between successive 2‑positions */

    for (idx_t i = 2; i <= n; ++i)
    {
        int s = (i == next_two ? 2 : 1); /* second difference at i */
        diff += s;
        a_curr = a_prev + diff;         /* a[i] = a[i-1] + d[i] */

        if (s == 2)                     /* schedule next 2‑position */
        {
            next_two += gap;
            ++gap;
        }

        a_prev = a_curr;
    }

    return (seq_t)a_curr;
}
