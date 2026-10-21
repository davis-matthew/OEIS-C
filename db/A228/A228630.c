/*
 * A228630  Number of s in {1,...,n}^n having shortest run with the same value
 *          of length 4.
 * https://oeis.org/A228630
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r=1}^{⌊n/4⌋} [C(n-3r-1,r-1) - C(n-4r-1,r-1)] * n *
 *            (n-1)^{r-1}
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 25, u64 n <= 43, u128 n <= 76,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(a,b) is the binomial coefficient, and (n-1)^{k} is a power in seq_t. */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is not defined; return 0. */
    if (n < 4)
        return 0;

    seq_t result = 0;

    for (idx_t r = 1; r <= n / 4; ++r)
    {
        /* Compute the two binomial coefficients when defined. */
        int64_t a1 = (int64_t)n - 3 * (int64_t)r - 1;
        if (a1 < (int64_t)(r - 1))
            continue; /* binomial undefined, skip to next r. */

        seq_t comp1 = binomial((idx_t)a1, r - 1);

        int64_t a2 = (int64_t)n - 4 * (int64_t)r - 1;
        seq_t comp2 = 0;
        if (a2 >= (int64_t)(r - 1))
            comp2 = binomial((idx_t)a2, r - 1);

        seq_t diff = comp1 - comp2;
        if (diff == 0)
            continue; /* no compositions with a run of length 4. */

        /* Compute n * (n-1)^{r-1}. */
        seq_t pow_term = pow_uint((uint64_t)(n - 1), r - 1);
        seq_t run_assign = (seq_t)n * pow_term;

        seq_t add = diff * run_assign;
        result += add;
    }

    return result;
}
