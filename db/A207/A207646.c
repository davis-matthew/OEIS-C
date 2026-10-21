/*
 * A207646  Product_{k=1..n} floor(2*n/k - 1).
 * https://oeis.org/A207646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} floor(2*n/k - 1) (with a(0) = 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

#define MAXSPF 200001   /* enough for n up to 10000 (2*n-1 <= 20000) */

static uint32_t spf[MAXSPF];
static int spf_prepared = 0;

/* Precompute smallest prime factors up to MAXSPF-1 */
static void prepare_spf(void)
{
    if (spf_prepared)
        return;
    for (uint32_t i = 0; i < MAXSPF; ++i)
        spf[i] = i;
    for (uint32_t i = 2; i * i < MAXSPF; ++i) {
        if (spf[i] == i) {            /* i is prime */
            for (uint32_t j = i * i; j < MAXSPF; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }
    spf_prepared = 1;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    prepare_spf();

    if (n == 0)
        return 1;

    /* maximum value that can appear in the factors */
    uint32_t max_val = (uint32_t)(2 * n - 1);
    if (max_val + 1 > MAXSPF) {
        /* should not happen for the intended n range */
        return 0;
    }

    /* exponent array for primes up to max_val */
    static uint32_t exp_arr[MAXSPF];
    memset(exp_arr, 0, (size_t)(max_val + 1) * sizeof(exp_arr[0]));

    seq_t two_n = (seq_t)n * 2;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t div = two_n / (seq_t)k;
        seq_t term = div - 1;           /* floor(2*n/k) - 1 */
        uint32_t t = (uint32_t)term;    /* term fits in uint32_t for n <= 10000 */

        /* factorization using precomputed spf */
        while (t > 1) {
            uint32_t p = spf[t];
            ++exp_arr[p];
            t /= p;
        }
    }

    seq_t result = 1;
    for (uint32_t p = 2; p <= max_val; ++p) {
        if (exp_arr[p] > 0) {
            seq_t pow = big_pow((seq_t)p, (seq_t)exp_arr[p]);
            result *= pow;
        }
    }

    return result;
}
