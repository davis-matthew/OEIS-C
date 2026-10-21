/*
 * A079822  Smallest n-digit multiple of the n-th composite number.
 * https://oeis.org/A079822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = ceil(10^(n-1)/c(n))*c(n), where c(n) is the n‑th
 *            composite number
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   isPrime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/isPrime.h"

/* Cache for composite numbers to avoid recomputation between calls. */
static uint64_t *composite_cache = NULL;
static size_t composite_cache_len = 0;
static size_t composite_cache_cap = 0;
static uint64_t next_candidate = 4; /* next integer to test for compositeness */

/* Return the n-th composite number (n >= 1). */
static uint64_t nth_composite(idx_t n)
{
    while (composite_cache_len < n)
    {
        uint64_t cand = next_candidate++;
        if (cand > 1 && !isPrime((idx_t)cand))
        {
            if (composite_cache_len == composite_cache_cap)
            {
                composite_cache_cap = composite_cache_cap ? composite_cache_cap * 2 : 16;
                composite_cache = (uint64_t *)realloc(composite_cache,
                                                      composite_cache_cap * sizeof(uint64_t));
                if (!composite_cache) abort(); /* fatal if allocation fails */
            }
            composite_cache[composite_cache_len++] = cand;
        }
    }
    return composite_cache[n - 1];
}

seq_t a(idx_t n)
{
    uint64_t c = nth_composite(n);          /* nth composite */
    seq_t start = pow10(n - 1);             /* 10^(n-1) */
    seq_t cseq  = (seq_t)c;
    seq_t one   = (seq_t)1;

    seq_t q = (start + cseq - one) / cseq;  /* ceil division: ceil(start / c) */
    seq_t ans = q * cseq;                   /* smallest n-digit multiple of c */
    return ans;
}
