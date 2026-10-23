/*
 * A031613  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 25.
 * https://oeis.org/A031613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer k such that the continued
 *            fraction of sqrt(k) has odd period and, after deleting the last
 *            term of that period, the two central terms are both 25
 * Range:     u8 none, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static seq_t *memo = NULL;          /* cached sequence items */
static size_t memo_size = 0;        /* number of items in memo   */
static size_t memo_cap  = 0;        /* allocated capacity        */
static uint64_t next_k   = 2;       /* next k to test */

static void grow_memo(size_t needed)
{
    if (needed <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    memo = new_mem;
    memo_cap = new_cap;
}

/* Test whether k satisfies the required property. */
static bool good_k(uint64_t k)
{
    /* Skip squares */
    uint64_t a0 = (uint64_t)sqrt((double)k);
    if (a0 * a0 == k) return false;

    /* Compute continued fraction expansion of sqrt(k) */
    uint64_t m = 0, d = 1;
    uint64_t a = a0;
    /* We'll store the periodic part in a small dynamic array. */
    static uint32_t *period = NULL;
    static size_t p_cap = 0;
    size_t p_len = 0;

    /* Ensure enough capacity for typical periods. */
    if (p_cap < 1024) {
        p_cap = 2048;
        period = (uint32_t *)realloc(period, p_cap * sizeof(uint32_t));
    }

    while (1) {
        if (p_len == p_cap) {
            p_cap <<= 1;
            period = (uint32_t *)realloc(period, p_cap * sizeof(uint32_t));
        }
        /* Next term of the period */
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;
        period[p_len++] = (uint32_t)a;
        if (a == 2 * a0) break;          /* period finished */
    }

    /* Check required conditions */
    if (p_len & 1) {                      /* odd period */
        size_t mid1 = (p_len - 2) / 2;
        size_t mid2 = p_len / 2;
        if (period[mid1] == 25 && period[mid2] == 25) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms computed */
    while (memo_size < n) {
        if (good_k(next_k)) {
            grow_memo(memo_size + 1);
            memo[memo_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return memo[n - 1];
}
