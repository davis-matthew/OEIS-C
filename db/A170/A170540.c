/*
 * A170540  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(n,46)} c_k * a(n–k) + (n ≤ 46 ? 1 : 0) where
 *            c_k = 2 if k is odd –1 if k is even Sequence is defined with
 *            a(0)=1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* a(n) returns the n‑th term of OEIS A170540.
   seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    /* cache up to the largest n requested so far, up to 10000 */
    static seq_t cache[10001];
    static idx_t cached = 0;   /* number of terms already computed */

    if (n < cached)
        return cache[n];

    for (idx_t i = cached; i <= n; ++i) {
        seq_t val = 0;
        size_t limit = (i <= 46) ? i : 46;   /* number of previous terms to use */

        for (size_t k = 1; k <= limit; ++k) {
            /* coefficient: 2 for odd k, –1 for even k */
            if (k & 1) {
                val += cache[i - k] * (seq_t)2;
            } else {
                val -= cache[i - k];
            }
        }

        if (i <= 46)      /* add the constant term for 0 ≤ i≤46 */
            val += (seq_t)1;

        cache[i] = val;
    }

    cached = n + 1;
    return cache[n];
}
