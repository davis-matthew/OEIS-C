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
 * Validated: standard, pass, 48 terms through n = 9649 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

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
                seq_t *tmp = mem_resize(seq_t, seq, newcap);
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
