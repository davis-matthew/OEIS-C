/*
 * A130141  Let f denote the map that replaces k with the concatenation of its
 *          nontrivial divisors, written in decreasing order, each divisor
 *          being written in base 10 in the normal way. Then a(n) = prime
 *          reached when starting at 2n+1 and iterating f.
 * https://oeis.org/A130141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   digit_len, factor_u64, is_prime_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130141
   a(n) = prime reached by starting at 2n+1 and iterating f where
   f(k) = concatenation of k's non‑trivial divisors (excluding 1 and k)
   written in decreasing order, each divisor in decimal.
   If the process never reaches a prime, a(n) = -1.
   If a number with more than 90 digits is produced, a(n) = 0. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

static void gen_divs(int idx, uint64_t curr, uint64_t m,
                     uint64_t p[], uint64_t e[], size_t pcnt,
                     uint64_t *divs, size_t *dcnt, size_t cap)
{
    if (idx == (int)pcnt) {
        if (curr != 1 && curr != m && *dcnt < cap)
            divs[(*dcnt)++] = curr;
        return;
    }
    uint64_t val = curr;
    for (uint64_t exp = 0; exp <= e[idx]; ++exp) {
        gen_divs(idx + 1, val, m, p, e, pcnt, divs, dcnt, cap);
        val *= p[idx];
    }
}

static int cmp_desc(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? 1 : (va > vb) ? -1 : 0;
}

seq_t a(idx_t n)
{
    /* start value 2n+1 */
    uint64_t start = 2ULL * (uint64_t)n + 1ULL;
    seq_t m = (seq_t)start;

    /* loop until we reach a prime or hit limits */
    for (int iter = 0; iter < 1000; ++iter) {
        if (m == 1)
            return 1;
        if (digit_len(m) > 90)
            return 0;
        uint64_t m64 = (uint64_t)m;
        if (m <= (seq_t)UINT64_MAX && is_prime_small(m64)) /* fits 64‑bit */
            return m;

        /* factor m (m fits in uint64_t for n up to 10000) */
        uint64_t primes[64], exps[64];
        size_t pf_cnt = 0;
        factor_u64(m64, primes, exps, &pf_cnt);
        if (pf_cnt == 0)                    /* prime or 1 */
            return m;

        /* generate all non‑trivial divisors */
        uint64_t divs[512];
        size_t dcnt = 0;
        gen_divs(0, 1ULL, m64, primes, exps, pf_cnt,
                 divs, &dcnt, 512);

        if (dcnt == 0)
            return m;      /* prime, but handled earlier */

        /* sort descending */
        qsort(divs, dcnt, sizeof(uint64_t), cmp_desc);

        /* build concatenated number */
        seq_t result = 0;
        for (size_t i = 0; i < dcnt; ++i) {
            seq_t d = (seq_t)divs[i];
            result = result * pow10(digit_len(d)) + d;
        }
        m = result;
    }
    /* if we exceed iteration limit, give up */
    return (seq_t)-1;
}
