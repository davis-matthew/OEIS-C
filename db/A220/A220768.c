/*
 * A220768  Number of tilings of a 2 X n rectangle using integer-sided
 *          rectangular tiles of equal area.
 * https://oeis.org/A220768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220768
   a(n) = Σ_{d|2n} T(n,d)
   where
       T(0,d) = 1
       T(k,d) = T(k-d,d)  +  (k>=d/2 ? T(k-d/2,d) : 0)    if d is even
                T(k,d) = T(k-d,d)                            if d is odd
   The function below implements this divisor‑sum recurrence.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Allocate/free arrays of seq_t that work in both C and C++         */
/* ------------------------------------------------------------------ */
static inline seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* ------------------------------------------------------------------ */
/*  Recursive divisor generation                                       */
/* ------------------------------------------------------------------ */
static void
generate_divisors(const uint64_t *primes, const uint64_t *exps,
                  size_t pcnt, size_t idx, uint64_t cur,
                  uint64_t *out, size_t *outcnt)
{
    if (idx == pcnt) {
        out[(*outcnt)++] = cur;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t val = cur;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        generate_divisors(primes, exps, pcnt, idx + 1, val, out, outcnt);
        val *= p;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* 2*n must fit in uint64_t for n up to 10 000 (given the limits) */
    uint64_t m = 2 * (uint64_t)n;

    /* Prime factorisation of m */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    uint64_t tmp = m;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            size_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Enumerate all divisors of 2*n */
    uint64_t divs[256];
    size_t dcnt = 0;
    generate_divisors(primes, exps, pcnt, 0, 1, divs, &dcnt);

    /* DP array for one divisor at a time */
    seq_t *dp = alloc_seq_array((size_t)n + 1);
    if (!dp) return (seq_t)0;   /* unlikely */

    seq_t sum = 0;

    for (size_t di = 0; di < dcnt; ++di) {
        uint64_t d = divs[di];
        dp[0] = (seq_t)1;

        for (idx_t i = 1; i <= n; ++i) {
            seq_t val = 0;
            if ((idx_t)i >= d) {
                val += dp[(size_t)(i - d)];
            }
            if ((d & 1ULL) == 0ULL && (idx_t)i >= (d >> 1)) {
                val += dp[(size_t)(i - (d >> 1))];
            }
            dp[(size_t)i] = val;
        }
        sum += dp[(size_t)n];
    }

    free_seq_array(dp);
    return sum;
}
