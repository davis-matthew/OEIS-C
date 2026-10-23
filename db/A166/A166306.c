/*
 * A166306  Denominator of Bernoulli_n multiplied by the sum of the associated
 *          inverse primes in the Staudt-Clausen theorem, n=1, 2, 4, 6, 8,
 *          10,...
 * https://oeis.org/A166306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p-1 | m} m_den/m where m = 1 if n=1, 2 if n=2,
 *            otherwise m = 2*n-2, and m_den = ∏_{p-1|m} p (m_den is the
 *            denominator of the Bernoulli number B_m)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 90, u64 n <= 270,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;   /* primes up to the current limit */
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* compute a(n) for the OEIS sequence A166306 */
seq_t a(idx_t n)
{
    /* Map the sequence index n to the Bernoulli index m */
    idx_t m;
    if (n == 1)
        m = 1;
    else if (n == 2)
        m = 2;
    else
        m = 2 * n - 2;      /* n >= 3 */

    idx_t limit = m + 1;     /* maximum prime we need to test */

    /* Build or extend the prime table when necessary */
    if (prime_list == NULL || limit > prime_limit) {
        /* free previous table if it exists */
        if (prime_list != NULL)
            free(prime_list);
        build_primes(limit, &prime_list, &prime_count);
        prime_limit = limit;
    }

    /* Collect primes p with p-1 | m and compute the product D */
    seq_t D = 1;
    const size_t MAX_SEL = 256;                 /* safe upper bound */
    idx_t selected[MAX_SEL];
    size_t sel_cnt = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > limit)
            break;                             /* no more relevant primes */

        if (m % (p - 1) == 0) {                /* p-1 divides m */
            D *= (seq_t)p;                     /* accumulate denominator */
            if (sel_cnt < MAX_SEL)
                selected[sel_cnt++] = p;       /* store the prime for later */
        }
    }

    /* Compute the sum Σ D/p over the selected primes */
    seq_t result = 0;
    for (size_t i = 0; i < sel_cnt; ++i)
        result += D / (seq_t)selected[i];

    return result;
}
