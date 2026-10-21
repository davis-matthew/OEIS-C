/*
 * A077480  Total number of prime factors of numbers m with bigomega(m) == 0
 *          modulo omega(m) (counted with repetition).
 * https://oeis.org/A077480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the bigomega‐value of the n-th integer m for which
   bigomega(m) is divisible by omega(m).  The sequence is OEIS A077480.
   The definition is: a(n) = ω(m_n)  where m_n is the n‑th integer (m ≥ 2)
   satisfying bigomega(m) ≡ 0 (mod omega(m)).  The ordering of the
   integers is ascending. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"      /* void build_spf(idx_t limit, idx_t **spf_out) */

static uint16_t *bigomega_arr = NULL;   /* bigomega for each m */
static uint16_t *omega_arr    = NULL;   /* omega   for each m  */
static idx_t     computed_limit = 0;     /* size of the arrays   */

static void compute_arrays(idx_t limit)
{
    /* allocate arrays */
    free(bigomega_arr);
    free(omega_arr);
    bigomega_arr = (uint16_t *)calloc(limit + 1, sizeof(uint16_t));
    omega_arr    = (uint16_t *)calloc(limit + 1, sizeof(uint16_t));
    if (!bigomega_arr || !omega_arr)
        abort();

    /* build smallest prime factor array */
    idx_t *spf = NULL;
    build_spf(limit, &spf);
    if (!spf)
        abort();

    /* compute bigomega and omega for every number */
    bigomega_arr[1] = 0;
    omega_arr[1]    = 0;
    for (idx_t i = 2; i <= limit; ++i)
    {
        idx_t x = i;
        uint16_t big = 0, om = 0;
        int prev = 0;
        while (x > 1)
        {
            int p = (int)spf[x];
            ++big;
            if (p != prev)
            {
                ++om;
                prev = p;
            }
            x /= p;
        }
        bigomega_arr[i] = big;
        omega_arr[i]    = om;
    }
    free(spf);
    computed_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* safety: offset starts at 1 */
        return 0;

    /* Quick return if already cached and sufficient */
    if (computed_limit) {
        idx_t count = 0;
        for (idx_t i = 2; i <= computed_limit; ++i) {
            if (omega_arr[i] && bigomega_arr[i] % omega_arr[i] == 0) {
                ++count;
                if (count == n)
                    return (seq_t)bigomega_arr[i];
            }
        }
        /* Not found, need more numbers */
    }

    /* If not computed yet or not enough, grow the limit */
    idx_t limit = computed_limit ? computed_limit : n * 3 + 100;
    while (1) {
        compute_arrays(limit);

        idx_t count = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (omega_arr[i] && bigomega_arr[i] % omega_arr[i] == 0) {
                ++count;
                if (count == n)
                    return (seq_t)bigomega_arr[i];
            }
        }
        /* still not enough, double the limit and try again */
        limit *= 2;
    }
}
