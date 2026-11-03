/*
 * A071324  Alternating sum of all divisors of n; divisors nonincreasing,
 *          starting with n.
 * https://oeis.org/A071324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = alternating sum of divisors of n, sorted in
 *            non‑increasing order (start with n). That is, if d_1> d_2 > … >
 *            d_k are the divisors of n, then a(n) = d_1 - d_2 + d_3 - d_4 + …
 *            (with d_1 = n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9744 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Comparator for ascending sort of seq_t values. */
static int seq_cmp_asc(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Factor n into arrays pr[ ] (primes) and exp[ ] (exponents).
   The arrays are pre‑allocated with at least 64 entries. */
static void factor_uint64(uint64_t n, uint64_t *pr, int *exp, size_t *cnt)
{
    size_t c = 0;
    uint64_t m = n;

    if ((m & 1ULL) == 0ULL) {
        pr[c] = 2ULL;
        int e = 0;
        while ((m & 1ULL) == 0ULL) { m >>= 1; e++; }
        exp[c] = e;
        c++;
    }

    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0ULL) {
            pr[c] = p;
            int e = 0;
            while (m % p == 0ULL) { m /= p; e++; }
            exp[c] = e;
            c++;
        }
    }

    if (m > 1ULL) {
        pr[c] = m;
        exp[c] = 1;
        c++;
    }

    *cnt = c;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (not in the offset range). */
    if (n == 0) return 0;

    uint64_t n64 = (uint64_t)n;

    /* Factor n. */
    uint64_t primes[64];
    int exps[64];
    size_t prime_cnt = 0;
    factor_uint64(n64, primes, exps, &prime_cnt);

    /* Generate all divisors.  Start with divisor 1. */
    size_t div_cap = 64;
    seq_t *divs = mem_new(seq_t, div_cap);
    size_t div_cnt = 1;
    divs[0] = (seq_t)1;

    for (size_t idx = 0; idx < prime_cnt; idx++) {
        uint64_t p = primes[idx];
        int e   = exps[idx];

        /* Compute successive powers of p: p^1, p^2, ... p^e. */
        seq_t pp = 1;
        seq_t *new_divs = mem_new(seq_t, (div_cnt * e));
        size_t new_cnt = 0;

        for (int k = 1; k <= e; k++) {
            pp *= (seq_t)p;
            for (size_t j = 0; j < div_cnt; j++) {
                new_divs[new_cnt++] = divs[j] * pp;
            }
        }

        /* Append the new divisors. */
        if (div_cnt + new_cnt > div_cap) {
            div_cap = div_cnt + new_cnt;
            divs = mem_resize(seq_t, divs, div_cap);
        }
        for (size_t j = 0; j < new_cnt; j++) {
            divs[div_cnt + j] = new_divs[j];
        }
        div_cnt += new_cnt;
        mem_free(new_divs);
    }

    /* Sort divisors in ascending order.  The largest divisor n is
       at the end; we will iterate backwards to get descending order. */
    mem_qsort(divs, div_cnt, sizeof(seq_t), seq_cmp_asc);

    /* Alternate sum starting with the largest divisor. */
    seq_t sum = 0;
    bool add = true;   /* true → add, false → subtract */
    for (size_t i = 0; i < div_cnt; i++) {
        seq_t d = divs[div_cnt - 1 - i];
        if (add) sum += d;
        else     sum -= d;
        add = !add;
    }

    mem_free(divs);
    return sum;
}
