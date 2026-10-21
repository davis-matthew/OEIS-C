/*
 * A081447  Smallest squares such that partial sums of the sequence plus 5 are
 *          primes.
 * https://oeis.org/A081447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation: For each n, a(n) is the smallest square
 * that is a multiple of 36, (6m)^2, such that the cumulative sum of the
 * first n terms plus 5 is a prime. This guarantees a(1)=36, a(2)=576,
 * etc., matching OEIS A081447. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

#define MAX_CACHE 100000   /* sufficient for n up to about 10000 */

static seq_t terms[MAX_CACHE];   /* memoised terms; index 0 corresponds to a(1) */
static idx_t cached_n = 0;       /* number of terms currently stored */
static seq_t total_sum = 0;      /* sum of all stored terms */

/* Return the nth term (1‑based) of sequence A081447 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined; safeguard */

    /* compute any missing terms up to n */
    while (cached_n < n)
    {
        /* find smallest (6m)^2 making the new sum +5 prime */
        seq_t chosen = 0;
        for (idx_t m = 1; ; ++m)
        {
            seq_t m_sq = (seq_t)m * (seq_t)m;   /* m^2 */
            seq_t square = 36 * m_sq;           /* (6m)^2 */
            seq_t test = total_sum + square + (seq_t)5;
            if (is_prime_big(test))
            {
                chosen = square;
                break;
            }
        }

        terms[cached_n] = chosen;
        total_sum += chosen;
        ++cached_n;
    }

    return terms[n - 1];
}
