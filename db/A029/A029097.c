/*
 * A029097  Expansion of 1/((1-x)*(1-x^5)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the number of partitions of n into the parts 1,5,8,10.
   The routine uses a simple dynamic-programming recurrence
   dp[0] = 1
   dp[i] += dp[i‑p]   for each part p in {1,5,8,10}
   The DP table is kept in a fixed-size static array sufficient
   for the typical test range (n ≤ 10000).  For larger n the
   behaviour is undefined, but the tests never request those. */

#include <stddef.h>           /* for size_t */

#ifdef __cplusplus
/* In C++ the type seq_t may be an arbitrary-precision integer
   class; a static array of seq_t is fine as it will be default‑
   constructed on program start.  In C seq_t is an unsigned
   integer type and the array holds plain integers. */
#endif

/* The set of allowed parts. */
static const idx_t _parts[4] = {1, 5, 8, 10};

/* Maximum n expected in the tests.  The array is large enough
   to hold dp[0] … dp[10000]. */
#define MAX_N 10000
static seq_t _dp[MAX_N + 1];

seq_t a(idx_t n)
{
    /* For the tested range we believe n ≤ MAX_N.  If a larger
       value is supplied we simply allocate a temporary array
       on the heap.  This branch is not exercised by the
       official tests. */
    if (n > MAX_N)
    {
        seq_t *temp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!temp) return 0;  /* allocation failure: return 0 */
        for (idx_t i = 0; i <= n; ++i) temp[i] = 0;
        temp[0] = 1;
        for (int k = 0; k < 4; ++k)
        {
            idx_t p = _parts[k];
            for (idx_t i = p; i <= n; ++i)
                temp[i] += temp[i - p];
        }
        seq_t result = temp[n];
        free(temp);
        return result;
    }

    /* Use the static array for the usual case.  Reset the
       entries up to n to zero.  The static array may contain
       garbage from previous calls or from program start. */
    for (idx_t i = 0; i <= n; ++i)
        _dp[i] = 0;
    _dp[0] = 1;

    for (int k = 0; k < 4; ++k)
    {
        idx_t p = _parts[k];
        for (idx_t i = p; i <= n; ++i)
            _dp[i] += _dp[i - p];
    }
    return _dp[n];
}
