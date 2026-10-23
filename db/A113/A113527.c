/*
 * A113527  Semiprimes in A056108.
 * https://oeis.org/A113527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*k^2 + k + 1 for the successive k where the value is a *
 *            semiprime. The returned sequence is the intersection of A056108
 *            and * A001358. This function yields the n‑th term (1‑based
 *            indexing)
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_semiprime_uint64.h"

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;   /* stored terms */
    static idx_t sz     = 0;      /* number of terms stored */
    static idx_t cap    = 0;      /* allocated capacity */
    static idx_t next_k = 1;      /* next candidate k to test */

    while (sz < n)
    {
        seq_t k   = (seq_t)next_k;
        seq_t val = 3 * k * k + k + 1;            /* 3*k^2 + k + 1 */

        /* Test semiprimeness; val is < 2^64 for the n-range required. */
        if (is_semiprime_uint64((uint64_t)val))
        {
            if (sz == cap)
            {
                idx_t newcap = cap == 0 ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;  /* allocation failure */
                seq = tmp;
                cap = newcap;
            }
            seq[sz++] = val;
        }
        next_k++;   /* advance candidate k for next iteration */
    }

    return seq[n - 1];
}
