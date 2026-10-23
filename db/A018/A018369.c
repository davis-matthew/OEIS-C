/*
 * A018369  Divisors of 276.
 * https://oeis.org/A018369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 276
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Recursive helper to generate all divisors of 276. */
static void gen_divs(int idx, seq_t cur,
                     const uint64_t primes[], const int exps[],
                     int cnt_factors, seq_t *divs, int *dcount)
{
    if (idx == cnt_factors) {
        divs[(*dcount)++] = cur;
        return;
    }
    seq_t val = 1;            /* prime^e 0..exps[idx] */
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs(idx + 1, cur * val, primes, exps, cnt_factors, divs, dcount);
        val *= (seq_t)primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* Factor 276 = 2^2 * 3^1 * 23^1. */
    const uint64_t primes_arr[3] = {2, 3, 23};
    const int exps_arr[3]      = {2, 1, 1};
    const int factor_cnt = 3;

    /* Precompute all divisors into an array. */
    /* Max number of divisors for 276 is 12, so 32 is ample. */
    seq_t divs[32];
    int dcount = 0;
    gen_divs(0, (seq_t)1, primes_arr, exps_arr, factor_cnt, divs, &dcount);

    /* Simple insertion sort (divisors are very few). */
    for (int i = 1; i < dcount; ++i) {
        seq_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            if (j == 0) { j = -1; break; }
            --j;
        }
        divs[j + 1] = key;
    }

    /* Return the requested divisor, or 0 if n is out of range. */
    if ((idx_t)n == 0 || n > (idx_t)dcount) {
        return (seq_t)0;
    }
    return divs[n - 1];
}
