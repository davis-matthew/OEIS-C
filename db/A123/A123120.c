/*
 * A123120  Numbers k>0 such that m+k is not the sum of m nonzero squares for
 *          any m>5.
 * https://oeis.org/A123120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123120 – Numbers k>0 such that m+k is not the sum of m
 * nonzero squares for any m>5.
 *
 * Observation:  The condition is equivalent to k not being a sum of
 * numbers of the form a^2-1 (a≥2).  All such numbers with a^2-1≡0
 * (mod 3) or 2 (mod 3) generate every integer ≥14 as a linear
 * combination of 3 and 8.  Consequently every k≥14 is representable
 * and only the following 7 integers are not:
 *   1,2,4,5,7,10,13
 *
 * Therefore the n-th term of the sequence is the n-th element of that
 * list.  The function below computes it on demand without using a
 * pre‑stored table, following the rules of the problem statement.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_representable(seq_t k)
{
    /* Every k ≥ 14 is guaranteed to be representable: 14 = 3·2 + 8·1.
       For smaller k we test all combinations of 3 and 8.    */
    if (k >= 14u)
        return true;

    for (idx_t b = 0; b * 8u <= k; ++b)
    {
        seq_t rem = k - 8u * b;
        if (rem % 3u == 0u)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Count non‑representable integers until we reach the n‑th one. */
    idx_t nonrep = 0;
    for (idx_t k = 1; ; ++k)
    {
        if (!is_representable(k))
        {
            ++nonrep;
            if (nonrep == n)
                return (seq_t)k;
        }

        /* All integers ≥14 are representable; beyond that no new
           non‑representable numbers can appear.  If we have not yet
           reached the requested n we return 0 as a sentinel value. */
        if (k >= 14u)
        {
            if (nonrep < n)
                return 0;
            else
                /* Shouldn't happen, but keep the loop form consistent */
                return 0;
        }
    }
}
