/*
 * A018735  Divisors of 944.
 * https://oeis.org/A018735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018735: Divisors of 944.
   a(n) is the n‑th divisor of 944 listed in increasing order. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Recursively generate all divisors of 944. */
static void
gen_divs(const uint64_t *primes, const uint64_t *exps, size_t cnt,
         size_t idx, seq_t current, seq_t *out, size_t *pos)
{
    if (idx == cnt) {
        out[(*pos)++] = current;
        return;
    }
    seq_t p = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divs(primes, exps, cnt, idx + 1, current * p, out, pos);
        p *= (seq_t)primes[idx];
    }
}

/* Comparison function for qsort. */
static int
cmp_seq(const void *x, const void *y)
{
    seq_t a = *(const seq_t *)x;
    seq_t b = *(const seq_t *)y;
    return (a < b) ? -1 : (a > b);
}

/* The main sequence routine. */
seq_t a(idx_t n)
{
    /* 944 has 10 divisors; we store them after first use. */
    static seq_t divisors[10];
    static bool initialized = false;

    if (!initialized) {
        /* Prime factorisation of 944: 2^4 * 59^1. */
        uint64_t target = 944ULL;
        uint64_t primes[8], exps[8];
        size_t cnt = 0;
        uint64_t m = target;

        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                uint64_t e = 0;
                while (m % p == 0) {
                    m /= p;
                    ++e;
                }
                primes[cnt] = p;
                exps[cnt] = e;
                ++cnt;
            }
        }
        if (m > 1) {            /* remaining prime factor */
            primes[cnt] = m;
            exps[cnt] = 1;
            ++cnt;
        }

        /* Count number of divisors: Π (exp+1). */
        size_t total = 1;
        for (size_t i = 0; i < cnt; ++i)
            total *= (exps[i] + 1);

        /* Generate all divisors into a temporary buffer. */
        seq_t tmp[16];          /* total <= 16 for a 64‑bit number */
        size_t pos = 0;
        gen_divs(primes, exps, cnt, 0, (seq_t)1, tmp, &pos);

        /* Sort the divisors. */
        qsort(tmp, pos, sizeof(seq_t), cmp_seq);

        /* Copy into the static array. */
        for (size_t i = 0; i < pos; ++i)
            divisors[i] = tmp[i];

        initialized = true;
    }

    if (n == 0 || n > 10)
        return 0;             /* undefined outside the sequence range */

    return divisors[n - 1];
}
