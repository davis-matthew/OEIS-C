/*
 * A076386  Numbers n such that sum of digits in base 8 is a divisor of sum of
 *          prime divisors (A008472).
 * https://oeis.org/A076386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076386: a(n) = the nth positive integer such that
   sum of digits in base 8 divides the sum of distinct prime divisors
   of n.  The code generically computes terms by scanning integers
   incrementally, caching all found terms to answer future queries in
   O(1). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* sum of digits of n in base 8 */
static inline seq_t sum_octal(idx_t n)
{
    seq_t sum = 0;
    idx_t t = n;
    while (t) {
        sum += (seq_t)(t & 7);   /* t % 8 */
        t >>= 3;                 /* t /= 8   */
    }
    return sum;
}

static seq_t *a_cache = NULL;   /* cached sequence terms          */
static size_t a_cap   = 0;
static size_t a_len   = 0;
static idx_t next_num = 2;      /* next integer to test          */

/* sum of distinct prime divisors of n (using factor_u64) */
static inline seq_t sum_prime_divs(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    seq_t s = 0;
    for (size_t i = 0; i < cnt; ++i)
        s += (seq_t)primes[i];
    return s;
}

/* extend the cache so that it contains at least n terms */
static void ensure_terms(idx_t n)
{
    while (a_len < n) {
        idx_t cand = next_num++;

        seq_t sum8 = sum_octal(cand);
        if (sum8 == 0)   /* should not happen for cand >= 1 */
            continue;

        seq_t sumP = sum_prime_divs(cand);

        if (sumP % sum8 == 0)
            append_candidate(&a_cache, &a_len, &a_cap, (seq_t)cand);
    }
}

/* External entry point required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)   /* defensive guard – offset is 1 */
        return (seq_t)0;

    ensure_terms(n);
    return a_cache[n - 1];   /* zero‑based indexing of cached terms */
}
