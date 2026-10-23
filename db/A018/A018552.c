/*
 * A018552  Divisors of 616.
 * https://oeis.org/A018552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 616 (1-indexed)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of divisors of 616 in ascending order. */
#include <stdbool.h>
#include <stdlib.h>

#define N_DIVISORS 16

/* Comparator for qsort on seq_t values. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t*)p1;
    seq_t v2 = *(const seq_t*)p2;
    return (v1 < v2) ? -1 : (v1 > v2);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static bool initialized = false;
    static seq_t divisors[N_DIVISORS];

    if (!initialized)
    {
        /* Prime factorisation of 616: 2^3 * 7^1 * 11^1. */
        static const unsigned long long primes[3]   = {2ULL, 7ULL, 11ULL};
        static const int             exps[3]      = {3, 1, 1};

        size_t idx = 0;
        for (int e0 = 0; e0 <= exps[0]; ++e0)
        {
            unsigned long long d0 = 1ULL;
            for (int i = 0; i < e0; ++i) d0 *= primes[0];

            for (int e1 = 0; e1 <= exps[1]; ++e1)
            {
                unsigned long long d1 = d0;
                for (int i = 0; i < e1; ++i) d1 *= primes[1];

                for (int e2 = 0; e2 <= exps[2]; ++e2)
                {
                    unsigned long long d2 = d1;
                    for (int i = 0; i < e2; ++i) d2 *= primes[2];

                    divisors[idx++] = (seq_t)d2;  /* cast to seq_t; result fits in seq_t for builds where seq_t can hold 616 */
                }
            }
        }

        /* Sort the divisors in ascending order. */
        qsort(divisors, N_DIVISORS, sizeof(seq_t), cmp_seq_t);

        initialized = true;
    }

    if (n > N_DIVISORS) return (seq_t)0;   /* undefined for n > 16 */
    return divisors[n-1];
}
