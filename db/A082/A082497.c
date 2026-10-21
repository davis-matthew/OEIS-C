/*
 * A082497  a(0)=0, a(1)=1, a(2n)=2a(n), a(2n+1)=a(n)+a(n-1).
 * https://oeis.org/A082497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2n)=2*a(n), a(2n+1)=a(n)+a(n-1)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static idx_t size = 0;          /* number of computed elements */
    static idx_t cap   = 0;          /* allocated capacity */

#ifdef __cplusplus
    static std::vector<seq_t> arr;
#else
    static seq_t *arr = NULL;
#endif

    /* enlarge storage if necessary */
    if (cap <= n)
    {
        idx_t newcap = cap ? cap << 1 : 2;
        while (newcap <= n)
            newcap <<= 1;
#ifdef __cplusplus
        arr.resize(newcap);          /* value‑initialise new elements (zero) */
#else
        seq_t *tmp = (seq_t *)realloc(arr, newcap * sizeof(seq_t));
        if (!tmp) return 0;         /* allocation failed – abort gracefully */
        arr = tmp;
#endif
        cap = newcap;
    }

    /* compute missing values up to n */
    for (idx_t i = size; i <= n; ++i)
    {
        if (i == 0)
            arr[0] = 0;
        else if (i == 1)
            arr[1] = 1;
        else if ((i & 1) == 0)          /* even: a(2k)=2*a(k) */
            arr[i] = (seq_t)2 * arr[i >> 1];
        else                            /* odd : a(2k+1)=a(k)+a(k-1) */
            arr[i] = arr[i >> 1] + arr[(i >> 1) - 1];
    }
    size = n + 1;

    return arr[n];
}
