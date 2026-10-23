/*
 * A022567  Expansion of Product_{m>=1} (1+x^m)^2.
 * https://oeis.org/A022567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{T_k = k*(k+1)/2, 0 ≤ T_k ≤ n} p(n - T_k)
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 115, u64 n <= 390,
 *            u128 n <= 1400, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *p_cache = NULL;
static idx_t p_cap = 0;
static idx_t p_last = 0;

/* Ensure that partition numbers up to n are computed */
static void ensure_partitions_up_to(idx_t n)
{
    if (p_cache == NULL) {
        /* at least capacity for p[0] */
        p_cap = 1;
        p_cache = alloc_seq(p_cap);
        p_cache[0] = (seq_t)1;   /* p(0) = 1 */
        p_last = 0;
    }

    if (n <= p_last)
        return;

    /* Expand the array if necessary */
    if (n + 1 > p_cap) {
        idx_t new_cap = p_cap ? p_cap : 1;
        while (new_cap <= n + 1)
            new_cap <<= 1;       /* double until large enough */

        seq_t *new_cache = alloc_seq(new_cap);
        for (idx_t i = 0; i <= p_last; ++i)
            new_cache[i] = p_cache[i];
        free_seq(p_cache);
        p_cache = new_cache;
        p_cap = new_cap;
    }

    /* Compute p[cur] for cur = p_last+1 .. n */
    for (idx_t cur = p_last + 1; cur <= n; ++cur) {
        seq_t sum_odd = 0;
        seq_t sum_even = 0;
        for (idx_t k = 1; ; ++k) {
            /* First generalized pentagonal number: k*(3k-1)/2 */
            idx_t gp1 = k * (3 * k - 1) / 2;
            if (gp1 > cur)
                break; /* gp2 will also be > cur, so we can stop */

            seq_t v1 = p_cache[cur - gp1];
            if (k & 1)
                sum_odd += v1;
            else
                sum_even += v1;

            /* Second generalized pentagonal number: k*(3k+1)/2 */
            idx_t gp2 = k * (3 * k + 1) / 2;
            if (gp2 <= cur) {
                seq_t v2 = p_cache[cur - gp2];
                if (k & 1)
                    sum_odd += v2;
                else
                    sum_even += v2;
            }
        }
        /* p(cur) = sum_odd - sum_even  (always non‑negative) */
        seq_t val = sum_odd;
        val -= sum_even;
        p_cache[cur] = val;
    }

    p_last = n;
}

seq_t a(idx_t n)
{
    ensure_partitions_up_to(n);

    seq_t sum = 0;
    for (idx_t k = 0; ; ++k) {
        idx_t t = k * (k + 1) / 2;   /* triangular number T_k */
        if (t > n)
            break;
        sum += p_cache[n - t];
    }
    return sum;
}
