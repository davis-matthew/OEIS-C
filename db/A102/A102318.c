/*
 * A102318  A mean binomial transform of the Catalan numbers.
 * https://oeis.org/A102318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k=0..⌊n/2⌋} binomial(n,2k) * Catalan(n-2k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for Catalan numbers up to n. */
    seq_t *cat = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!cat)
        return 0;                       /* allocation failure – return 0 */

    cat[0] = 1;
    for (idx_t k = 1; k <= n; ++k)
        cat[k] = cat[k - 1] * (seq_t)(4 * k - 2) / (seq_t)(k + 1);

    seq_t sum = 0;
    seq_t binom_val = 1;                 /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k > 0)
            binom_val = binom_val * (seq_t)(n - k + 1) / (seq_t)(k);

        if ((k & 1) == 0)               /* even k → add term */
            sum += binom_val * cat[(size_t)(n - k)];
    }

    free(cat);
    return sum;
}
