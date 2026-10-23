/*
 * A184705  Number of strings of numbers x(i=1..5) in 0..n with sum i*x(i)
 *          equal to n*5.
 * https://oeis.org/A184705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5‑tuples (x1,…,x5) with 0 ≤ xi ≤ n and x1 +
 *            2·x2 + 3·x3 + 4·x4 + 5·x5 = 5·n
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;                 /* a(0) is not defined – return zero */

    idx_t target = 5U * n;              /* the required sum */

    /* cache for unrestricted counts f[0 … target]  */
    static seq_t *f = NULL;
    static idx_t f_size = 0U;

    /* enlarge cache if necessary */
    if (target >= f_size) {
        seq_t *tmp = (seq_t *)realloc(f, (target + 1U) * sizeof(seq_t));
        if (tmp == NULL)
            abort();                    /* out of memory */
        f = tmp;
        f_size = target + 1U;
    }

    /* initialise f[0…target] to zero and set f[0] = 1 */
    for (idx_t j = 0; j <= target; ++j)
        f[j] = 0;
    f[0] = 1;

    /* coin‑change DP for coins 1…5 (unbounded) */
    for (idx_t coin = 1; coin <= 5; ++coin) {
        for (idx_t j = coin; j <= target; ++j)
            f[j] += f[j - coin];
    }

    /* inclusion–exclusion over subsets of {1,2,3,4,5} */
    seq_t pos = 0;                       /* sum over even‑sized subsets */
    seq_t neg = 0;                       /* sum over odd‑sized subsets  */
    idx_t n_plus1 = n + 1U;

    for (idx_t mask = 0; mask < (1U << 5U); ++mask) {
        idx_t k = 0;                     /* amount to subtract from target */
        unsigned bits = __builtin_popcountll(mask);

        if (mask & 1U)  k += 1U * n_plus1;
        if (mask & 2U)  k += 2U * n_plus1;
        if (mask & 4U)  k += 3U * n_plus1;
        if (mask & 8U)  k += 4U * n_plus1;
        if (mask & 16U) k += 5U * n_plus1;

        if (k > target)
            continue;                   /* no solutions for this subset */

        idx_t m = target - k;           /* RHS of the unrestricted equation */

        if ((bits & 1U) == 0U)
            pos += f[m];
        else
            neg += f[m];
    }

    return pos - neg;                    /* final count, guaranteed non‑negative */
}
