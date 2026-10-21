/*
 * A096864  Function A062402(x) = sigma(phi(x)) is iterated. Starting with n,
 *          a(n) is the largest term arising in trajectory, either in
 *          transient or in terminal cycle.
 * https://oeis.org/A096864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = max(n, A066437(n)), where A066437(n) is the maximum value
 *            in the trajectory of repeated application of f(x) =
 *            sigma(phi(x)) starting from n. This implementation iterates the
 *            map until a repetition occurs, tracking the greatest element
 *            seen. It uses the provided calc_phi routine and impleme...
 * Range:     u8 n <= 84, u16 n <= 3384, u32 n <= 9581, u64 n <= 9581,
 *            u128 n <= 9581, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"   /* provides seq_t calc_phi(seq_t m); */

/* ------------------ sum of divisors sigma(x) --------------------- */
static seq_t sigma_seq(seq_t n)
{
    if (n <= 1)
        return n;            /* sigma(0)=0, sigma(1)=1 */

    uint64_t tmp   = (uint64_t)n;
    uint64_t result = 1ULL;

    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            uint64_t power   = 1ULL;
            uint64_t sum     = 1ULL;
            while (tmp % p == 0)
            {
                tmp   /= p;
                power *= p;
                sum   += power;
            }
            result *= sum;
        }
    }
    if (tmp > 1)            /* remaining prime factor */
        result *= (1ULL + tmp);

    return (seq_t)result;
}

/* ------------------------- main sequence ------------------------ */
seq_t a(idx_t n)
{
    /* A fixed capacity is sufficient for the required range. */
    enum { SEEN_CAP = 256 };
    seq_t seen[SEEN_CAP];
    size_t seen_len = 0;

    seq_t current = (seq_t)n;
    seq_t maxi    = current;

    /* record the starting value */
    seen[seen_len++] = current;

    for (;;)
    {
        seq_t phi  = calc_phi(current);
        seq_t next = sigma_seq(phi);

        if (next > maxi)
            maxi = next;

        /* linear search for repetition */
        bool repeated = false;
        for (size_t i = 0; !repeated && i < seen_len; ++i)
            if (seen[i] == next)
                repeated = true;

        if (repeated)
            break;          /* closed cycle – maximum already seen */

        /* store new value if capacity allows */
        if (seen_len < SEEN_CAP)
            seen[seen_len++] = next;
        else
            break;          /* safety: should not happen for n ≤ 10000 */

        current = next;
    }

    return maxi;
}
