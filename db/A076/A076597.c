/*
 * A076597  Numbers k such that sqrt(k*(k-1)*(k-2)*(k-3)+1) is a prime.
 * https://oeis.org/A076597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 443, u32 n <= 65, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k^2 - 3*k + 1 is prime
   (equivalent to sqrt(k*(k-1)*(k-2)*(k-3)+1) being prime). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static idx_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 4;

static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    idx_t *tmp = (idx_t*)realloc(seq_cache, new_cap * sizeof(idx_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* External function returned by the evaluation harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0, not used by OEIS */
    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        seq_t k = (seq_t)next_k;                 /* current candidate k   */
        seq_t val = k * k - 3 * k + 1;           /* k^2 - 3k + 1          */
        if (is_prime_big(val))
        {
            ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len++] = next_k;    /* store the integer k   */
        }
        ++next_k;
    }
    return (seq_t)seq_cache[n - 1];              /* convert stored int to seq_t */
}
