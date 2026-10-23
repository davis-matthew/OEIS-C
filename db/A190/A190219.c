/*
 * A190219  Numbers all of whose divisors have decimal digits in strictly
 *          decreasing order.
 * https://oeis.org/A190219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 72, u32 n <= 104, u64 n <= 104, u128 n <= 104,
 *            big any n
 * Helpers:   factor_u64, has_decreasing_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A190219 (Numbers all of whose divisors have decimal digits in strictly decreasing order).
   The sequence is finite; at most 104 terms exist.  The function generates all
   strictly decreasing-digit numbers up to 98765431, checks each one's divisors, and
   stores the qualifying numbers in ascending order for subsequent lookups. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/has_decreasing_digits.h"

static seq_t *s_seq = NULL;
static size_t s_count = 0;

/* Compare function for qsort on seq_t */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Check whether all divisors of num (uint64_t) have decreasing digits */
static bool all_divisors_ok(uint64_t num)
{
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(num, pr, ex, &cnt);

    /* Generate all divisors */
    uint64_t divs[2000];
    size_t div_count = 1;
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t base = pr[i];
        uint64_t pow = 1;
        size_t prev = div_count;
        for (int e = 1; e <= (int)ex[i]; ++e) {
            pow *= base;
            for (size_t j = 0; j < prev; ++j) {
                divs[div_count++] = divs[j] * pow;
            }
        }
    }

    /* Test each divisor */
    for (size_t i = 0; i < div_count; ++i) {
        if (!has_decreasing_digits(divs[i])) return false;
    }
    return true;
}

/* Compute the whole sequence once */
static void compute_sequence(void)
{
    if (s_seq) return;           /* already computed */

    /* Temporary array large enough for all candidates */
    seq_t *tmp = (seq_t *)malloc(2048 * sizeof(seq_t));
    size_t tmp_len = 0;

    /* Enumerate all strictly decreasing-digit numbers (except 0) */
    for (int mask = 1; mask < (1 << 10); ++mask) {
        if (mask == 1) continue;          /* exclude number 0 */

        seq_t val = 0;
        for (int d = 9; d >= 0; --d) {
            if (mask & (1 << d)) {
                val = val * 10 + d;
            }
        }
        uint64_t num = (uint64_t)val;
        if (all_divisors_ok(num)) {
            tmp[tmp_len++] = val;
        }
    }

    /* Sort ascending */
    qsort(tmp, tmp_len, sizeof(seq_t), cmp_seq);

    /* Store as static sequence */
    s_seq = tmp;
    s_count = tmp_len;
}

seq_t a(idx_t n)
{
    if (!s_seq) compute_sequence();   /* ensure precomputation */

    if (n == 0) return 0;             /* undefined, return 0 */
    if (n <= s_count) return s_seq[n - 1];
    return 0;                         /* out of range → 0 */
}
