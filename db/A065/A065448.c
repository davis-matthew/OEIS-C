/*
 * A065448  Numbers k such that k is congruent to the product of its digits
 *          modulo the sum of its digits.
 * https://oeis.org/A065448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th integer k ≥ 1 such that k ≡ (product of the
 *            decimal digits of k) (mod sum of the decimal digits of k). The
 *            sequence is computed by scanning the positive integers in
 *            increasing order and collecting those that satisfy the
 *            congruence. A static cache is used so that arbitrary calling...
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t  cache_len = 0;      /* number of terms currently stored              */
static idx_t  cache_cap = 0;      /* allocated capacity of the cache               */
static void*  raw_cache = NULL;   /* raw pointer returned by realloc, casted to seq_t* when used */
static seq_t  next_k    = 1;      /* smallest integer not yet examined             */

/* enlarge the cache if needed */
static void ensure_cache(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    while (new_cap < needed)
        new_cap <<= 1;
    raw_cache = realloc(raw_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* the requested nth term (1‑based) */
seq_t a(idx_t n)
{
    /* offset is 1 – behaviour for n == 0 is irrelevant */
    if (n == 0)
        return 0;

    ensure_cache(n);
    seq_t* cache = (seq_t*)raw_cache;   /* cast is safe for C and C++ */

    while (cache_len < n) {
        seq_t k = next_k;
        seq_t sum = 0;
        seq_t temp = k;

        /* first pass: compute the sum of digits */
        while (temp > 0) {
            seq_t d = temp % 10;
            sum += d;
            temp /= 10;
        }

        /* second pass: compute the product of digits modulo 'sum' */
        seq_t prod = 1;
        temp = k;
        while (temp > 0) {
            seq_t d = temp % 10;
            /* 'sum' is positive because k > 0 */
            prod = (prod * d) % sum;
            temp /= 10;
        }

        if (k % sum == prod % sum) {
            cache[cache_len++] = k;
        }
        ++next_k;
    }

    return cache[n - 1];   /* a(1) is the first stored term */
}
