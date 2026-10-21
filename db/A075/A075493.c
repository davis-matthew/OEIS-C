/*
 * A075493  Numbers k such that (sum of digits of k) > (number of divisors of
 *          k).
 * https://oeis.org/A075493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = the nth positive integer k with digit_sum(k) > divisor_count(k).
 * The function builds the sequence incrementally, searching for the next
 * qualifying integer each time it is needed.  It uses an on‑the‑fly integer
 * divisor count routine that works for values that fit in uint64_t.
 */

#include <stdlib.h>
#include <stdint.h>

/* local helper: digit sum of an idx_t value */
static inline seq_t digit_sum_idx(idx_t x)
{
    seq_t s = 0;
    while (x) {
        s += (seq_t)(x % 10);
        x /= 10;
    }
    return s;
}

/* local helper: divisor count for uint64_t values */
static inline seq_t divisor_count_uint64(uint64_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t cnt = (seq_t)1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            seq_t exp = (seq_t)0;
            while (n % p == 0) {
                n /= p;
                exp += (seq_t)1;
            }
            cnt *= (exp + (seq_t)1);
        }
    }
    if (n > 1) cnt *= (seq_t)2;
    return cnt;
}

/* dynamic storage for the terms that have already been found */
static seq_t *terms = NULL;
static idx_t  terms_count = 0;
static idx_t  terms_cap   = 0;

/* integer that we test next */
static idx_t current_k = 0;

seq_t a(idx_t n)
{
    if (n == 0)   /* strictly defensive – the sequence offset starts at 1 */
        return (seq_t)0;

    while (terms_count < n) {
        /* find the next integer satisfying the condition */
        for (;;) {
            ++current_k;
            seq_t sum   = digit_sum_idx(current_k);
            seq_t dcnt  = divisor_count_uint64((uint64_t)current_k);
            if (sum > dcnt) break;
        }

        /* grow the array if necessary */
        if (terms_cap <= terms_count) {
            idx_t new_cap = terms_cap ? terms_cap << 1 : 16;
            while (new_cap <= terms_count) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure – return sentinel */
            terms = tmp;
            terms_cap = new_cap;
        }

        /* store the found term (wraps on overflow – harmless for the builds that test it) */
        terms[terms_count++] = (seq_t)current_k;
    }

    return terms[n - 1];
}
