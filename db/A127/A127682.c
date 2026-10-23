/*
 * A127682  Number of non-isomorphic (i.e., defined up to a rotation and a
 *          reflection) maximal independent sets of the n-cycle graph having
 *          at least one symmetry axis. Also: Number of cyclic and palindromic
 *          compositions of n in which each term is either 2 or 3.
 * https://oeis.org/A127682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-4) + a(n-6) for n≥7, * with initial values: a(1)=0,
 *            a(2)=1, a(3)=1, * a(4)=1, a(5)=1, a(6)=2
 * Range:     u8 n <= 41, u16 n <= 81, u32 n <= 161, u64 n <= 317,
 *            u128 n <= 633, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *memo = NULL;
    static size_t memo_len = 0;   /* highest index computed (0 unused) */
    static size_t memo_cap = 0;   /* current allocated capacity */

    if (n == 0) return 0;  /* not used, safeguard */

    size_t ni = (size_t)n;

    /* expand cache if needed */
    if (ni > memo_cap) {
        size_t new_cap = (memo_cap == 0) ? 16 : memo_cap;
        while (new_cap < ni) new_cap <<= 1;   /* double until large enough */
        seq_t *new_mem = (seq_t *)realloc(memo, (new_cap + 1) * sizeof(seq_t));
        if (!new_mem) exit(1);  /* allocation failure */
        memo = new_mem;
        /* initialize the new entries to 0 */
        for (size_t i = memo_cap + 1; i <= new_cap; ++i) memo[i] = 0;
        memo_cap = new_cap;
    }

    /* compute values up to the requested index */
    if (ni > memo_len) {
        for (size_t i = memo_len + 1; i <= ni; ++i) {
            if (i == 1)       memo[i] = 0;
            else if (i == 2)  memo[i] = 1;
            else if (i == 3)  memo[i] = 1;
            else if (i == 4)  memo[i] = 1;
            else if (i == 5)  memo[i] = 1;
            else if (i == 6)  memo[i] = 2;
            else              memo[i] = memo[i - 4] + memo[i - 6];
        }
        memo_len = ni;
    }

    return memo[ni];
}
