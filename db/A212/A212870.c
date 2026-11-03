/*
 * A212870  Number of nondecreasing sequences of 3 1..n integers with no
 *          element dividing the sequence sum.
 * https://oeis.org/A212870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of nondecreasing triples (x,y,z) with 1≤x≤y≤z≤n such
 *            that none of x,y,z divides x+y+z
 * Range:     u8 n <= 13, u16 n <= 77, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Validated: standard, pass, 64 terms through n = 198 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Count number of nondecreasing triples containing a specific divisor d. */
static inline seq_t count_with_div(idx_t S, idx_t d, idx_t n) {
    long long T = (long long)S - (long long)d;      /* sum of the other two numbers */
    seq_t cnt = 0;

    /* case 1: d is the smallest element (a = d) */
    long long lower_b = (long long)d;
    long long t_minus_n = (T > (long long)n) ? T - (long long)n : 0;
    if (t_minus_n > lower_b) lower_b = t_minus_n;
    long long upper_b = T / 2;
    if (upper_b > (long long)n) upper_b = (long long)n;
    if (lower_b <= upper_b) cnt += (seq_t)(upper_b - lower_b + 1);

    /* case 2: d is the middle element (b = d), a != d */
    if (d > 1) {
        long long lower_a = 1;
        long long val = (T > (long long)n) ? T - (long long)n : 0;
        if (val > lower_a) lower_a = val;      /* a ≥ T - n for c ≤ n */

        long long upper_a = (long long)d - 1;   /* a ≤ d-1 to keep a < d */
        if (upper_a > T / 2) upper_a = T / 2;   /* enforce a ≤ floor(T/2) to keep b ≤ c */

        long long t_minus_d = T - (long long)d; /* a ≤ T - d to keep c ≥ d */
        if (t_minus_d < upper_a) upper_a = t_minus_d;

        if (lower_a <= upper_a) cnt += (seq_t)(upper_a - lower_a + 1);
    }

    /* case 3: d is the largest element (c = d), a & b < d */
    long long lower_a2 = 1;
    long long val2 = (T > (long long)d) ? T - (long long)d + 1 : 1;  /* a > T - d */
    if (val2 > lower_a2) lower_a2 = val2;
    long long upper_a2 = (long long)d - 1;   /* a ≤ d-1 */
    if (upper_a2 > T / 2) upper_a2 = T / 2; /* enforce a ≤ floor(T/2) for a ≤ b */

    if (lower_a2 <= upper_a2) cnt += (seq_t)(upper_a2 - lower_a2 + 1);

    return cnt;
}

static inline idx_t idx_max(idx_t a, idx_t b) { return a > b ? a : b; }

static inline idx_t idx_min(idx_t a, idx_t b) { return a < b ? a : b; }

static inline long long ll_max(long long a, long long b) { return a > b ? a : b; }

static inline long long ll_min(long long a, long long b) { return a < b ? a : b; }

static inline long long ll_min3(long long a, long long b, long long c) {
    return ll_min(a, ll_min(b, c));
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    idx_t maxS = 3 * n;              /* maximum possible sum of three numbers */
    size_t sz    = (size_t)maxS + 3 ;   /* array size covering indices 0..maxS+2 */

    /* --- compute total number of nondecreasing triples for each sum S --- */
    long long *diff = (long long*)calloc(sz, sizeof(long long));
    seq_t *total   = mem_new(seq_t, sz);
    if (!diff || !total) return (seq_t)0;

    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        for (idx_t b_val = a_val; b_val <= n; ++b_val) {
            idx_t L = a_val + b_val + b_val;          /* a + 2b  (c = b) */
            if (L > maxS) continue;
            idx_t R = a_val + b_val + n;              /* a + b + n (c = n) */
            if (R > maxS) R = maxS;

            diff[L] += 1;
            if (R + 1 < sz) diff[R + 1] -= 1;
        }
    }
    long long acc = 0;
    for (size_t i = 0; i <= (size_t)maxS; ++i) {
        acc += diff[i];
        total[i] = (seq_t)acc;
    }
    free(diff);

    /* --- now count allowed triples --- */
    seq_t result = 0;

    for (idx_t S = 3; S <= maxS; ++S) {
        seq_t totTriples = total[S];

        /* collect divisors of S that are ≤ n */
        idx_t divs[200];
        size_t divCnt = 0;
        for (idx_t d = 1; d * d <= S; ++d) {
            if (S % d == 0) {
                if (d <= n) divs[divCnt++] = d;
                idx_t e = S / d;
                if (e != d && e <= n) divs[divCnt++] = e;
            }
        }

        /* Sum for inclusion–exclusion */
        seq_t sum1 = 0;   /* Σ |Ei|  : triples containing divisor d   */
        for (size_t i = 0; i < divCnt; ++i)
            sum1 += count_with_div(S, divs[i], n);

        seq_t sum2 = 0;   /* Σ |Ei∩Ej| : triples containing both d and e  */
        for (size_t i = 0; i < divCnt; ++i) {
            for (size_t j = i + 1; j < divCnt; ++j) {
                idx_t d = divs[i], e = divs[j];
                idx_t w = S - d - e;
                if (w >= 1 && w <= n) sum2 += (seq_t)1;
            }
        }

        seq_t sum3 = 0;   /* Σ |Ei∩Ej∩Ek| : triples containing d,e,f  */
        for (size_t i = 0; i < divCnt; ++i) {
            for (size_t j = i + 1; j < divCnt; ++j) {
                for (size_t k = j + 1; k < divCnt; ++k) {
                    if (divs[i] + divs[j] + divs[k] == S) sum3 += (seq_t)1;
                }
            }
        }

        /* Inclusion–exclusion: forbidden = sum1 - sum2 + sum3 */
        seq_t allowedS = totTriples - sum1 + sum2 - sum3;
        result += allowedS;
    }

    mem_free(total);
    return result;
}
