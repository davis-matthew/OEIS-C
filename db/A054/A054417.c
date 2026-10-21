/*
 * A054417  Number of connected 3 X n binary matrices with rightmost column
 *          [1,0,0]'.
 * https://oeis.org/A054417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 3*a(n-2) + 5*a(n-3), with a(1)=1, a(2)=5,
 *            a(3)=37. The function is memoised and works for calls in any
 *            order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifndef NULL
# define NULL ((void*)0)
#endif

/* Helper: multiply a seq_t value by a small unsigned long long
   constant.  In C++ the implementation uses __int128 for
   integral types to avoid overflow; in C it always uses
   unsigned __int128.  For arbitrary-precision seq_t types
   the multiplication operator (*) is used. */
#ifndef __cplusplus
static inline seq_t mul_small(seq_t a, unsigned long long m)
{
    return (seq_t)(((unsigned __int128)a) * m);
}
#else
#include <type_traits>
static inline seq_t mul_small(seq_t a, unsigned long long m)
{
    if constexpr (std::is_integral<seq_t>::value) {
        return (seq_t)((__int128)a * m);
    } else {
        return a * (seq_t)m;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n=0, return 0 */

    /* Memoisation table, indexed from 0 (index 0 unused). */
    static seq_t *tbl = NULL;
    static idx_t cap = 0;
    static idx_t last = 0;

    /* Ensure capacity for index n. */
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? 4 : cap;
        while (n >= new_cap) new_cap <<= 1;   /* grow by factor 2 */
        seq_t *tmp = (seq_t *)realloc(tbl, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        tbl = tmp;
        cap = new_cap;
    }

    /* Initialise first three terms if not yet done. */
    if (last < 3) {
        tbl[0] = (seq_t)0;      /* unused but harmless */
        tbl[1] = (seq_t)1;
        tbl[2] = (seq_t)5;
        tbl[3] = (seq_t)37;
        last = 3;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t1 = mul_small(tbl[i - 1], 7);
        seq_t t2 = mul_small(tbl[i - 2], 3);
        seq_t t3 = mul_small(tbl[i - 3], 5);
        tbl[i] = t1 + t3 - t2;
    }
    last = n;
    return tbl[n];
}
