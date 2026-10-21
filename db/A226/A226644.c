/*
 * A226644  Number of ways to express 5/n as Egyptian fractions in just three
 *          terms; i.e., 5/n = 1/x + 1/y + 1/z satisfying 1<=x<=y<=z.
 * https://oeis.org/A226644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of solutions to 5/n = 1/x+1/y+1/z with 1<=x<=y<=z */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
/* Helper for factoring: Include the provided utility. */
#include "../utils/factor_u64.h"  /* This header may be correct for the helper function.  */

static void generate_divisors(const uint64_t *pr, const uint64_t *ex,
                              size_t pcnt, size_t idx, uint64_t cur,
                              uint64_t *arr, size_t *cnt)
{
    if (idx == pcnt) {
        arr[*cnt] = cur;
        ++*cnt;
        return;
    }
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        generate_divisors(pr, ex, pcnt, idx + 1, val, arr, cnt);
        val *= pr[idx];
    }
}

/* Sort of static cache for divisors: not needed but static array is fine */
static uint64_t divs_buf[20000];

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset=1 guard */

    seq_t count = 0;

    /* x must satisfy  n/5 < x <= 3n/5  */
    idx_t startx = n / 5 + 1;
    idx_t endx   = (3 * n) / 5;
    if (startx > endx) return 0;

    /* Temporary arrays for factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    uint64_t dbl_exps[64];
    size_t  fac_cnt;

    /* For every admissible x */
    for (idx_t x = startx; x <= endx; ++x) {
        /* Compute P = 5x - n  > 0  and Q = n*x  */
        uint64_t P = 5ULL * (uint64_t)x - (uint64_t)n;
        uint64_t Q = (uint64_t)n * (uint64_t)x;
        uint64_t Q2 = Q * Q;

        /* Factorise Q (using provided helper) */
        factor_u64(Q, primes, exps, &fac_cnt);

        /* Double each exponent for Q^2 */
        for (size_t i = 0; i < fac_cnt; ++i)
            dbl_exps[i] = exps[i] << 1;

        size_t div_cnt = 0;
        generate_divisors(primes, dbl_exps, fac_cnt, 0, 1ULL,
                          divs_buf, &div_cnt);

        for (size_t i = 0; i < div_cnt; ++i) {
            uint64_t u = divs_buf[i];
            uint64_t v = Q2 / u;
            if (u > v) continue;                 /* ensures y <= z */

            if ((u + Q) % P != 0) continue;
            if ((v + Q) % P != 0) continue;

            uint64_t y = (u + Q) / P;
            uint64_t z = (v + Q) / P;

            if (y < (uint64_t)x) continue;      /* x <= y  */
            if (z < y) continue;                /* y <= z  */

            count += (seq_t)1;
        }
    }

    return count;
}
