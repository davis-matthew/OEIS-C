/*
 * A056271  Number of primitive (aperiodic) words of length n which contain
 *          exactly six different symbols.
 * https://oeis.org/A056271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} μ(d) * A000920(n/d) where A000920(m) =
 *            sum_{i=0}^{6} (-1)^i * C(6,i) * (6-i)^m (onto words of length m)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the inclusion–exclusion expression for A000920
   and the Möbius function provided by the library.  */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius.h"   /* provides seq_t mobius(idx_t m) */

/* fast power for seq_t values */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* binomial coefficients C(6,i) */
static const int C6[7] = {1, 6, 15, 20, 15, 6, 1};

/* cached A000920 up to size A920_size-1 */
static seq_t *A920      = NULL;
static idx_t  A920_size = 0;

/* cached primitive counts up to cache_size-1 */
static seq_t *a_cache   = NULL;
static idx_t  cache_size = 0;

/* extend A920 up to n */
static void ensure_A920(idx_t n)
{
    if (n < 1) return;
    if (n < A920_size) return;

    size_t newsize = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)realloc(A920, newsize * sizeof(seq_t));
    if (!new_arr) return;                     /* allocation failure – leave as is */

    if (A920_size == 0)
        memset(new_arr, 0, newsize * sizeof(seq_t));

    for (idx_t k = A920_size; k <= n; ++k)
    {
        seq_t pos = 0, neg = 0;
        for (int i = 0; i <= 6; ++i)
        {
            seq_t term = pow_seq((seq_t)(6 - i), k);
            term *= (seq_t)C6[i];
            if ((i & 1) == 0)
                pos += term;
            else
                neg += term;
        }
        new_arr[k] = pos - neg;                /* guaranteed non‑negative */
    }

    A920 = new_arr;
    A920_size = newsize;
}

/* extend a(n) up to n */
static void ensure_a(idx_t n)
{
    if (n < 1) return;
    if (n < cache_size) return;

    size_t newsize = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)realloc(a_cache, newsize * sizeof(seq_t));
    if (!new_arr) return;                     /* allocation failure – leave as is */

    if (cache_size == 0)
        memset(new_arr, 0, newsize * sizeof(seq_t));

    /* make sure A920 values are ready */
    ensure_A920(n);

    for (idx_t m = cache_size; m <= n; ++m)
    {
        seq_t pos = 0, neg = 0;
        for (idx_t d = 1; d * d <= m; ++d)
        {
            if (m % d == 0)
            {
                int mu_d = (int)mobius(d);
                if (mu_d != 0)
                {
                    idx_t k = m / d;
                    seq_t v = A920[k];
                    if (mu_d > 0)
                        pos += v;
                    else
                        neg += v;
                }

                idx_t d2 = m / d;
                if (d2 != d)
                {
                    int mu_d2 = (int)mobius(d2);
                    if (mu_d2 != 0)
                    {
                        idx_t k2 = m / d2;   /* equals d */
                        seq_t v2 = A920[k2];
                        if (mu_d2 > 0)
                            pos += v2;
                        else
                            neg += v2;
                    }
                }
            }
        }
        new_arr[m] = pos - neg;               /* result is non‑negative */
    }

    a_cache = new_arr;
    cache_size = newsize;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_a(n);
    return a_cache[n];
}
