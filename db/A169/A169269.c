/*
 * A169269  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p(n) + 14 * Σ_{k=1}^{28} a(n-k) - 105 * a(n-29),
 * where
 *   p(0) = 1,
 *   p(k) = 2  for 1 ≤ k ≤ 28,
 *   p(k) = 0  for k ≥ 29.
 * This linear recurrence is derived from the generating function
 *   G(t) = (t^29 + 2t^28 + … + 2t + 1) /
 *          (1 - 14t - 14t^2 - … - 14t^28 + 105t^29).
 * The implementation memoises the values in a static array so that
 * repeated calls in any order are O(1) after the initial
 * precomputation.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* cache up to a(10000); sufficient for the required range */
    static seq_t cache[10001];
    static idx_t computed = 0;  /* next index to compute */

    if (n >= 10000) {
        /* n beyond the pre‑allocated range – not expected per spec */
        /* In a production setting one would grow the array */
        return (seq_t)0; /* fallback */
    }

    for (idx_t k = computed; k <= n; ++k)
    {
        /* compute p(k) */
        seq_t pk = (k == 0 ? (seq_t)1
                  : (k <= 28 ? (seq_t)2 : (seq_t)0));

        /* sum of the 28 previous terms */
        seq_t sum = 0;
        for (idx_t j = 1; j <= 28; ++j)
        {
            if (k >= j)
                sum += cache[k - j];
        }

        /* recurrence */
        seq_t term = pk + sum * (seq_t)14;
        if (k >= 29)
            term -= cache[k - 29] * (seq_t)105;

        cache[k] = term;
    }

    computed = n + 1;
    return cache[n];
}
