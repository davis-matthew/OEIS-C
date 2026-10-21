/*
 * A067148  Fibonacci-like sequences. a(n) is the number of pairs of integers
 *          (n,i), 0<i<n, with Property F: i and n are consecutive terms
 *          i=b(j-1) and n=b(j), for some j>2, of a sequence {b(k)} satisfying
 *          b(1)=1, b(2)>0 and b(k)=b(k-1)+b(k-2) for all k>2.
 * https://oeis.org/A067148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A067148.
   For a given n, the function counts the distinct integers i
   (0 < i < n) for which there exists a Fibonacci-like sequence
   with b(1)=1, b(2)>0, and b(k)=b(k-1)+b(k-2) (k>2) such that
   i and n are consecutive terms at some position j>2.
   The algorithm iterates over possible positions j, solves for
   the starting term b(2), and deduplicates the resulting i's.
   It is O(log n) per call, suitable for n up to at least 10 000. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t nn = (seq_t)n;          /* n as seq_t for calculations        */
    seq_t fib_t_minus1 = 0;       /* F(t-1), starting with F(0) = 0     */
    seq_t fib_t       = 1;        /* F(t),   starting with F(1) = 1     */
    seq_t fib_t_plus1 = 1;        /* F(t+1), starting with F(2) = 1     */

    seq_t i;
    seq_t count = 0;              /* number of distinct i’s found       */
    seq_t seen[64];               /* buffer for deduplication          */
    int seen_cnt = 0;

    while (fib_t < nn)           /* terminate when numerator would be ≤0 */
    {
        seq_t numerator = nn - fib_t;
        if (numerator % fib_t_plus1 == 0)
        {
            seq_t x = numerator / fib_t_plus1;   /* candidate b(2)  */
            if (x >= (seq_t)1)
            {
                i = fib_t * x + fib_t_minus1;    /* candidate i  */
                /* deduplicate */
                int j, dup = 0;
                for (j = 0; j < seen_cnt; ++j)
                    if (seen[j] == i) { dup = 1; break; }
                if (!dup)
                {
                    seen[seen_cnt++] = i;
                    ++count;
                }
            }
        }

        /* advance Fibonacci numbers for next k: t → t+1   */
        fib_t_minus1 = fib_t;
        fib_t = fib_t_plus1;
        fib_t_plus1 = fib_t + fib_t_minus1;
    }

    return count;
}
