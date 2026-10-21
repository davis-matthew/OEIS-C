/*
 * A067281  Number of permutations of {1,2,3,...,n} where the elements of n
 *          are considered indistinguishable if they differ by a power of 2
 *          (for example 3, 12 and 24 are all considered equivalent).
 * https://oeis.org/A067281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1,
 *          a(n)=a(n-1)*n / L,
 * where L is the length of the 2‑power chain that contains n
 * (i.e. the number of integers of the form r·2^k with r odd,
 * r·2^k ≤ n).  Equivalently,
 *     a(n) = n! / Π_{odd r ≤ n} (⌊log₂(n/r)⌋+1)!.
 * The implementation builds the values iteratively on first use
 * and memoises all results up to the current maximum index.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *memo = NULL;   /* dynamic array of seq_t values */
    static size_t memcap = 0;    /* current capacity */
    static idx_t latest = 0;     /* highest index that has been computed */

    /* initialise at first call */
    if (memo == NULL)
    {
        memcap = 16;
        memo = (seq_t *)malloc(memcap * sizeof(seq_t));
        memo[0] = (seq_t)1;
        latest = 0;
    }

    /* grow the memo array if necessary */
    if ((size_t)(n + 1) > memcap)
    {
        size_t newcap = memcap;
        while (newcap < (size_t)(n + 1))
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
        memo = tmp;
        memcap = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = latest + 1; i <= n; ++i)
    {
        /* a(i) = a(i-1) * i / newL */
        seq_t val = memo[i - 1];
        val *= (seq_t)i;                     /* multiply by i */

        /* find odd part r of i */
        idx_t r = i;
        while ((r & 1ULL) == 0) r >>= 1;

        /* compute new chain length L = floor(log2(i/r)) + 1 */
        idx_t y = i / r;                     /* i/r >= 1 */
        idx_t v = 0;
        while (y > 1)
        {
            y >>= 1;
            ++v;
        }
        idx_t newL = v + 1;                   /* L >= 1 */

        val /= (seq_t)newL;                   /* exact division */
        memo[i] = val;
    }
    latest = n;
    return memo[n];
}
