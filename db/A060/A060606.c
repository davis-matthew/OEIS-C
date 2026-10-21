/*
 * A060606  The n-th term is the sum of lengths of iteration chains to get
 *          fixed points (=1) for the Euler totient function from 1 to n.
 * https://oeis.org/A060606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j = 1}^{n+1} A003434(j) (offset 0)
 * Range:     u8 n <= 59, u16 n <= 6591, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Maximum N we support – the sequence is required only for n ≈ 10 000. */
#define MAX_N 20000

/* Static tables reused across calls. */
static seq_t phi[MAX_N + 1];
static seq_t len[MAX_N + 1];
static seq_t prefix[MAX_N + 1];
static idx_t computed = 0;  /* largest index for which tables are valid */

/* External linkage as requested. */
seq_t a(idx_t n)
{
    /* The formula uses (n+1) terms of A003434.  */
    idx_t target = n + 1;

    if (target == 0)                 /* n == UINT64_MAX, never happens, but guard. */
        return 0;

    /* Re‑compute tables up to the required size if necessary. */
    if (target > computed)
    {
        idx_t limit = target;
        if (limit > MAX_N)           /* safety check – should not occur for tests. */
            limit = MAX_N;

        /* Initialise phi[i] = i. */
        for (idx_t i = 1; i <= limit; ++i)
            phi[i] = (seq_t)i;

        /* Sieve for Euler's totient. */
        for (idx_t p = 2; p <= limit; ++p)
        {
            if (phi[p] == (seq_t)p)  /* p is prime */
            {
                for (idx_t j = p; j <= limit; j += p)
                {
                    phi[j] -= phi[j] / (seq_t)p;
                }
            }
        }

        /* Compute chain lengths. */
        len[1] = 0;
        for (idx_t i = 2; i <= limit; ++i)
            len[i] = len[(idx_t)phi[i]] + 1;

        /* Prefix sums of the lengths. */
        prefix[0] = 0;
        for (idx_t i = 1; i <= limit; ++i)
            prefix[i] = prefix[i - 1] + len[i];

        computed = limit;
    }

    /* Return the required sum. */
    return prefix[target];
}
