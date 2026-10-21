/*
 * A061130  Number of degree-n even permutations of order dividing 6.
 * https://oeis.org/A061130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of even permutations of order dividing 6. The
 *            recurrence follows from the exponential generating function
 *            exp(x + ½x² + ⅓x³ + ⅙x⁶) and its signed counterpart
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *even = NULL;          /* even‑parity counts */
    static seq_t *odd  = NULL;          /* odd‑parity counts  */
    static idx_t  cap   = 0;            /* allocated capacity  */
    static idx_t  done  = 0;            /* highest index computed */

    /* Allocate or grow tables to at least n+1 entries */
    if (n + 1 > cap)
    {
        idx_t newcap = (cap == 0) ? 1 : cap;
        while (newcap <= n) newcap <<= 1;

        seq_t *new_even = NULL;
        seq_t *new_odd  = NULL;

#ifdef __cplusplus
        new_even = new seq_t[newcap];
        new_odd  = new seq_t[newcap];
#else
        new_even = (seq_t*)malloc(sizeof(seq_t) * newcap);
        new_odd  = (seq_t*)malloc(sizeof(seq_t) * newcap);
#endif
        if (even)
        {
            for (idx_t i = 0; i < cap; ++i)
            {
                new_even[i] = even[i];
                new_odd[i]  = odd[i];
            }
#ifdef __cplusplus
            delete[] even;
            delete[] odd;
#else
            free(even);
            free(odd);
#endif
        }
        even = new_even;
        odd  = new_odd;
        cap  = newcap;
    }

    /* Initialise base case */
    if (done == 0)
    {
        even[0] = (seq_t)1;
        odd [0] = (seq_t)0;
        done = 0;
    }

    /* Compute values up to n */
    for (idx_t i = done + 1; i <= n; ++i)
    {
        seq_t e = even[i-1];          /* k = 1, parity unchanged */
        seq_t o = odd [i-1];

        /* multiplicative factors for k = 2, 3, 6 */
        seq_t m2 = (seq_t)(i - 1);                     /* for k=2   */
        seq_t m3 = m2 * (seq_t)(i - 2);                /* for k=3   */
        seq_t m6 = m3 * (seq_t)(i - 3); m6 *= (seq_t)(i - 4); m6 *= (seq_t)(i - 5); /* for k=6 */

        if (i >= 2)
        {
            e += m2 * odd[i-2];          /* k=2, parity flips */
            o += m2 * even[i-2];
        }
        if (i >= 3)
        {
            e += m3 * even[i-3];         /* k=3, parity unchanged */
            o += m3 * odd [i-3];
        }
        if (i >= 6)
        {
            e += m6 * odd[i-6];          /* k=6, parity flips */
            o += m6 * even[i-6];
        }

        even[i] = e;
        odd [i] = o;
    }

    done = n;
    return even[n];
}
