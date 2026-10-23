/*
 * A046641  a(n) is the smallest positive integer m such that the number of
 *          partitions p(m) = A000041(m) is divisible by n.
 * https://oeis.org/A046641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 8892, u32 n <= 9691, u64 n <= 9691,
 *            u128 n <= 9691, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) = the smallest m such that the partition number p(m)
 *  is divisible by n.  p(m) is computed modulo n using Euler's
 *  pentagonal‑number recurrence.  The routine works for any
 *  seq_t/idx_t types defined by the build (uint64_t, uint8_t, __int128,
 *  etc.).  All intermediate sums are performed in signed 64‑bit
 *  integers, which is safe for the expected input ranges. */
#include <stdlib.h>

/* Returns the smallest m such that p(m) is a multiple of n.
 * idx_t n >= 1.  The function can be called in any order.
 */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined, but return 0 to avoid division by zero. */
        return (seq_t)0;
    }

    /* Modulus as uint64_t for safe arithmetic. */
    const uint64_t mod = (uint64_t)n;

    /* Basic capacity for the partition table. */
    size_t cap = 128;
    uint64_t *p = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!p) {
        /* Allocation failed – return 0 as a sentinel. */
        return (seq_t)0;
    }

    /* p(0) = 1 mod n. */
    p[0] = 1 % mod;

    /* Main search loop. */
    for (idx_t m = 1; ; ++m) {
        /* Grow table if needed. */
        if ((size_t)m >= cap) {
            size_t new_cap = cap << 1;
            uint64_t *tmp = (uint64_t *)realloc(p, new_cap * sizeof(uint64_t));
            if (!tmp) {
                free(p);
                return (seq_t)0; /* allocation failure */
            }
            p = tmp;
            cap = new_cap;
        }

        /* Compute p(m) modulo n using pentagonal numbers. */
        int64_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            /* Generalized pentagonal numbers: g1 = k(3k-1)/2, g2 = k(3k+1)/2 */
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m && g2 > m) break;

            int64_t term = 0;
            if (g1 <= m) term += (int64_t)p[m - g1];
            if (g2 <= m) term += (int64_t)p[m - g2];

            if (k & 1) {
                sum += term;        /* add for odd k */
            } else {
                sum -= term;        /* subtract for even k */
            }
        }

        /* Reduce sum modulo n to a positive residue. */
        int64_t residue = sum % (int64_t)mod;
        if (residue < 0) residue += (int64_t)mod;

        p[m] = (uint64_t)residue;
        if (residue == 0) {
            free(p);
            return (seq_t)m;
        }
    }
}
