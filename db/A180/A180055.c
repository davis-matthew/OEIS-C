/*
 * A180055  Numbers k such that in binary expansion, the number of 1's in 5*k
 *          is less than the number of 1's in k.
 * https://oeis.org/A180055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 4314, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS A180055: numbers k such that
   popcount(5*k) < popcount(k).  a(1) = 13, a(2) = 26, …          */

#include <stdlib.h>
#include <stdint.h>

static int popcount_seq(seq_t x)
{
    int cnt = 0;
    while (x)
    {
        cnt += (int)(x & 1);
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* An offset of 1: a(1) is the first found number.  If
       n == 0 the result is undefined; we return 0.  */
    if (n == 0) return 0;

    /* Static state for memoisation.  */
    static seq_t *vals = NULL;          /* array of found terms          */
    static idx_t vals_cap = 0;           /* allocated capacity            */
    static idx_t found = 0;              /* number of terms already stored*/
    static uint64_t current_k = 1;       /* next candidate to test, starts 1 */

    /* Ensure capacity for at least n terms.  */
    if (vals_cap < n)
    {
        idx_t new_cap = (vals_cap == 0) ? 8 : vals_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        vals = tmp;
        vals_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (found < n)
    {
        seq_t k = (seq_t)current_k;
        seq_t k5 = k * 5;
        int pc_k  = popcount_seq(k);
        int pc_k5 = popcount_seq(k5);
        if (pc_k5 < pc_k)
        {
            vals[found] = k;
            ++found;
        }
        ++current_k;
    }

    return vals[n - 1];
}
