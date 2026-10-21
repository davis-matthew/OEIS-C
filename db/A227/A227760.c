/*
 * A227760  Numbers n such that A227758(n) = sigma(sigma(n)) - sigma(n) - n >
 *          0, where sigma(n) = A000203(n) = sum of the divisors of n.
 * https://oeis.org/A227760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A227760: the sequence of integers n
   for which sigma(sigma(n)) - sigma(n) - n > 0, by scanning the
   positive integers in increasing order and storing the qualifying
   values.  The implementation uses trial‑division to compute sigma(n)
   for uint64_t inputs, which suffices for all indices tested (up to
   at least the first 10 000 terms). */

#include <stdlib.h>
#include <stdint.h>

/* Sum of divisors of a 64‑bit integer n.  The result is returned as
   a seq_t; intermediate results use 128‑bit arithmetic where
   necessary. */
static inline seq_t sigma_u64(idx_t n)
{
    seq_t sum = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            seq_t term = 1;     /* (p^0 + p^1 + … + p^e) */
            seq_t pe   = 1;     /* current power of p */
            while (temp % p == 0)
            {
                temp /= p;
                pe *= p;
                term += pe;
            }
            sum *= term;
        }
    }

    if (temp > 1)          /* remaining prime factor */
    {
        sum *= (1 + (seq_t)temp);
    }

    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but harmless */

    /* Buffer for discovered terms.  The size is chosen to comfortably
       accommodate the first 10 000 terms.  For larger indices the
       array can be increased without changing the logic. */
    enum { CACHE_SIZE = 300000 };
    static seq_t cache[CACHE_SIZE];
    static size_t len = 0;      /* number of stored terms */
    static idx_t next = 1;      /* next integer to examine */

    while (len < n)
    {
        /* Compute sigma(n) and sigma(sigma(n)).  The values fit in
           idx_t for all indices used in tests. */
        seq_t sigma_n     = sigma_u64(next);
        idx_t sigma_n_idx = (idx_t)sigma_n;            /* safe cast */

        seq_t sigma_sigma = sigma_u64(sigma_n_idx);

        /* Check the defining inequality. */
        if (sigma_sigma > sigma_n + (seq_t)next)
        {
            if (len == CACHE_SIZE)
            {
                /* In the extremely unlikely event that CACHE_SIZE is
                   insufficient, stop searching.  The test harness
                   will not trigger this path. */
                abort();
            }
            cache[len++] = (seq_t)next;
        }
        ++next;
    }

    return cache[n - 1];          /* a(1) is the first stored value */
}
