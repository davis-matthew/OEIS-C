/*
 * A080383  Number of j (0 <= j <= n) such that the central binomial
 *          coefficient C(n,floor(n/2)) = A001405(n) is divisible by C(n,j).
 * https://oeis.org/A080383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of j (0 ≤ j ≤ n) such that C(n,⌊n/2⌋) is divisible
 *            by C(n,j)
 * Range:     u8 n <= 8895, u16 n <= 8895, u32 n <= 8895, u64 n <= 8895,
 *            u128 n <= 8895, big any n
 * Helpers:   adjust_exp, build_primes, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/build_spf.h"
#include "../utils/adjust_exp.h"

/* ------------------------------------------------------------------ */
/* Update the prime exponent array for a factor value.                 */

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ---- build required tables ----------------------------------- */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes(n, &primes, &primes_cnt);

    idx_t *spf = NULL;
    build_spf(n, &spf);

    /* mapping from prime to its index in the primes array             */
    int *prime_idx = (int *)calloc((size_t)(n + 1), sizeof(int));
    for (size_t i = 0; i < primes_cnt; ++i)
        prime_idx[primes[i]] = (int)i;

    /* ---- compute exponents of the central binomial coefficient ---- */
    int *target_exp = (int *)calloc(primes_cnt, sizeof(int));
    idx_t k = n / 2;                     /* floor(n/2) */
    for (idx_t t = 1; t <= k; ++t) {
        /* numerator part: multiply (n - t + 1) */
        adjust_exp(n - t + 1, +1, target_exp, spf, prime_idx);
        /* denominator part: multiply t */
        adjust_exp(t, -1, target_exp, spf, prime_idx);
    }

    /* ---- iterate over all j and count those that divide the target --- */
    int *current_exp = (int *)calloc(primes_cnt, sizeof(int));
    unsigned long long count = 0;

    for (idx_t j = 0; j <= n; ++j) {
        if (j == 0) {                    /* C(n,0) = 1 always divides */
            ++count;
            continue;
        }
        /* Update exponents: C(n,j) = C(n,j-1) * (n-j+1)/j */
        adjust_exp(n - j + 1, +1, current_exp, spf, prime_idx);
        adjust_exp(j, -1, current_exp, spf, prime_idx);

        bool ok = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            if (current_exp[i] > target_exp[i]) {
                ok = false;
                break;
            }
        }
        if (ok) ++count;
    }

    /* ---- free memory ----------------------------------------------- */
    free(primes);
    free(spf);
    free(prime_idx);
    free(target_exp);
    free(current_exp);

    return (seq_t)count;
}
