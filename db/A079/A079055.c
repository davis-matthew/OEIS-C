/*
 * A079055  Numbers of prime pairs (p,q), p<=q, such that (p+q) divides n.
 * https://oeis.org/A079055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A079055: a(n) = Sum_{d|n} A061358(d)
 *  Numbers of prime pairs (p,q), p <= q, such that (p+q) divides n.
 *
 *  Implementation detail:  For each unordered prime pair (p,q) with p+q ≤ N,
 *  we increment a[m] for every multiple m = (p+q)·k.  The value a[n] is then
 *  the number of such pairs whose sum divides n.
 *
 *  The routine stores the sequence a[1..Nmax] and the list of primes
 *  up to Nmax.  When a larger n is requested, the data structures are
 *  enlarged and the new contribution of the additional prime pairs is
 *  added to the affected indices.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    static idx_t Nmax = 0;          /* largest n processed so far              */
    static seq_t *A = NULL;         /* a[0..Nmax] (a[0] unused)                */
    static idx_t *primes = NULL;    /* list of primes up to Nmax               */
    static size_t prime_cnt = 0;    /* number of primes stored                 */

    if (n <= 1)                     /* offset is 1, a(1) = 0 by definition   */
        return 0;

    /* Extend precomputation if necessary */
    if (n > Nmax)
    {
        idx_t oldN = Nmax;
        Nmax = n;

        /* Allocate / extend array A and clear new entries */
        A = (seq_t *)realloc(A, (Nmax + 1) * sizeof(seq_t));
        for (idx_t i = oldN + 1; i <= Nmax; ++i)
            A[i] = 0;

        /* --- sieve for primes up to Nmax --- */
        bool *is_comp = (bool *)malloc((Nmax + 1) * sizeof(bool));
        memset(is_comp, 0, (Nmax + 1) * sizeof(bool));

        /* resize primes array (at most Nmax primes) */
        primes = (idx_t *)realloc(primes, (Nmax + 1) * sizeof(idx_t));
        prime_cnt = 0;

        for (idx_t i = 2; i <= Nmax; ++i)
        {
            if (!is_comp[i])
            {
                primes[prime_cnt++] = i;
                if (i * i <= Nmax)
                {
                    for (idx_t j = i * i; j <= Nmax; j += i)
                        is_comp[j] = true;
                }
            }
        }
        free(is_comp);

        /* --- add contributions of new prime pairs --- */
        for (size_t pi = 0; pi < prime_cnt; ++pi)
        {
            idx_t p = primes[pi];
            for (size_t qi = pi; qi < prime_cnt; ++qi)
            {
                idx_t q = primes[qi];
                idx_t s = p + q;           /* sum of the pair */
                if (s > Nmax) break;       /* further q only increase s */

                idx_t k_start = oldN / s + 1; /* first multiplier not yet counted */
                idx_t k_end   = Nmax / s;     /* last multiplier within new range */

                for (idx_t k = k_start; k <= k_end; ++k)
                {
                    idx_t idx = s * k;            /* n for which (p+q)|n */
                    A[idx] += (seq_t)1;
                }
            }
        }
    }

    return A[n];
}
