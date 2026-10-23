/*
 * A135766  Multiply sequence A007775 (1 7 11 13 ...) by sequence A000351 (1 5
 *          25 125 ...).
 * https://oeis.org/A135766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   Let r be the smallest positive integer with r(r+1)/2 >= n. Let k
 *            = n - (r-1)r/2 - 1 (so 0 <= k < r). Then a(n) = A007775(r-k) *
 *            5^k. Here A007775(1)=1 and for m>1, A007775(m) is the (m-1)-th
 *            prime ≥7
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----  caching of A007775 and prime generation  ---- */
static seq_t *A007775_cache = NULL;   /* 1‑based indexing, A007775(1)=1 */
static idx_t  A007775_cache_size = 0;
static uint64_t prime_candidate = 7;   /* next odd number to test for primality */

/* ensure that A007775_cache contains entries up to at least 'idx' */
static void ensure_A007775(idx_t idx)
{
    if (idx <= A007775_cache_size) return;

    size_t new_size = (size_t)(idx + 1);    /* allocate space for indices 0..idx */
    seq_t *new_cache = (seq_t *)realloc(A007775_cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();                  /* out of memory */
    A007775_cache = new_cache;
    for (idx_t i = A007775_cache_size + 1; i <= idx; ++i)
    {
        seq_t val;
        if (i == 1)
        {
            val = (seq_t)1;
        }
        else
        {
            /* find next prime ≥7 */
            while (true)
            {
                uint64_t n = prime_candidate;
                bool is_prime = true;
                for (uint64_t d = 3; d * d <= n; d += 2)
                {
                    if (n % d == 0)
                    {
                        is_prime = false;
                        break;
                    }
                }
                if (is_prime)
                {
                    val = (seq_t)n;
                    prime_candidate += 2;
                    break;
                }
                prime_candidate += 2;
            }
        }
        A007775_cache[i] = val;
    }
    A007775_cache_size = idx;
}

/* ----  main sequence function  ---- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* n>=1 by specification */

    /* find r such that r(r+1)/2 >= n > (r-1)r/2 */
    idx_t r = 0;
    idx_t tri = 0;
    while (tri < n)
    {
        ++r;
        tri += r;
    }
    idx_t prev_tri = tri - r;          /* sum of 1..(r-1) */
    idx_t k = n - prev_tri - 1;        /* 0‑based position in row r */
    idx_t j = r - k;                   /* index for A007775 (1‑based) */

    ensure_A007775(j);                 /* ensure cache has A007775(j) */

    /* compute 5^k */
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow5 *= (seq_t)5;

    return A007775_cache[j] * pow5;
}
