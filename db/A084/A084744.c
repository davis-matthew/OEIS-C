/*
 * A084744  Product of all composite numbers from 1 to the n-th nonprime
 *          number divided by product of all the prime divisors of each of
 *          those composite numbers which exceed the previously stated value.
 * https://oeis.org/A084744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A084744: product of all composite numbers up to the (n‑th) nonprime
 *  divided by the product of their squarefree kernels (radicals).
 *
 *  Define a(1) = 1.  For n > 1, list the integers m > 1 for which
 *  m / rad(m) > 1, and multiply the first n‑1 such ratios.
 *  This is equivalent to the OEIS definition:
 *
 *      a(n) = ∏{m ≤ M, m composite, m/rad(m)>1} (m / rad(m))
 *  where M is the smallest integer such that exactly n‑1 numbers
 *  satisfy the condition.
 *
 *  The function uses a simple SPF sieve to obtain rad(m).
 *
 *  Complexity: O(limit log log limit) with limit ≈ n / 0.39.
 */

#include <stdlib.h>
#include <stdint.h>

static void build_spf(idx_t limit, idx_t **out)
{
    idx_t *arr = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!arr) { *out = NULL; return; }
    for (idx_t i = 0; i <= limit; ++i) arr[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (arr[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (arr[j] == j) arr[j] = i;
            }
        }
    }
    *out = arr;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    idx_t need = n - 1;          /* number of ratio factors to multiply */
    idx_t limit = (n < 20) ? 100 : (idx_t)(n * 3 + 1000);  /* initial estimate */

    while (1) {
        idx_t *spf = NULL;
        build_spf(limit, &spf);
        if (!spf) return (seq_t)0;        /* allocation failure – should not occur */

        idx_t counter = 0;
        seq_t prod = (seq_t)1;

        for (idx_t m = 4; m <= limit && counter < need; ++m) {
            if (spf[m] == m) continue;     /* skip primes */

            /* compute rad(m) */
            idx_t temp = m;
            idx_t rad = 1;
            idx_t last_p = 0;
            while (temp > 1) {
                idx_t p = spf[temp];
                if (p != last_p) {
                    rad *= p;
                    last_p = p;
                }
                temp /= p;
            }

            idx_t ratio = m / rad;          /* m / rad(m) > 1 for the numbers we keep */
            if (ratio > 1) {
                prod *= (seq_t)ratio;
                ++counter;
            }
        }

        free(spf);

        if (counter >= need) return prod;   /* found enough factors */

        /* not enough – extend the sieve limit and try again */
        limit *= 2;
    }
}
