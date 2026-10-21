/*
 * A209445  a(n) = Pell(n)*A001227(n) for n >= 1, where A001227(n) is the
 *          number of odd divisors of n.
 * https://oeis.org/A209445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n) * A001227(n) where Pell(0)=0, Pell(1)=1,
 *            Pell(n)=2*Pell(n-1)+Pell(n-2) and A001227(n) counts the odd
 *            divisors of n
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   odd_div_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/odd_div_count.h"

static seq_t *pell_cache = NULL;   /* Pell numbers */
static idx_t pell_len = 0;          /* number of entries stored (indices 0 .. pell_len-1) */

static seq_t *odd_cache = NULL;    /* odd divisor counts */
static idx_t odd_len = 0;           /* size of odd_cache (entries 0 .. odd_len-1) */

/* Ensure pell_cache has entries up to n (inclusive). */
static void ensure_pell(idx_t n)
{
    if (pell_len <= n) {
        size_t new_len = (pell_len == 0) ? 2 : pell_len * 2;
        if (new_len <= n) new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(pell_cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        pell_cache = tmp;
        if (pell_len == 0) {
            pell_cache[0] = 0;
            pell_cache[1] = 1;
            pell_len = 2;
        }
        for (size_t i = pell_len; i < new_len; ++i) {
            pell_cache[i] = (seq_t)2 * pell_cache[i - 1] + pell_cache[i - 2];
        }
        pell_len = new_len;
    }
}

/* Ensure odd_cache has entries up to n (inclusive). */
static void ensure_odd(idx_t n)
{
    if (odd_len <= n) {
        size_t new_len = (odd_len == 0) ? 1 : odd_len * 2;
        if (new_len <= n) new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(odd_cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        odd_cache = tmp;
        if (odd_len == 0) {
            odd_cache[0] = 0;   /* unused, keep alignment */
            odd_len = 1;
        }
        for (size_t i = odd_len; i < new_len; ++i) {
            odd_cache[i] = odd_div_count((idx_t)i);
        }
        odd_len = new_len;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_pell(n);
    ensure_odd(n);
    return pell_cache[n] * odd_cache[n];
}
