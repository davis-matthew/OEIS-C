/*
 * A071804  Number of pairs (x,y) such that tau(x^y) = x+y, 1<=x<=n, 1<=y<=n.
 * https://oeis.org/A071804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y) | 1 ≤ x ≤ n, 1 ≤ y ≤ n, tau(x^y) = x + y }
 * Range:     u8 n <= 50, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Build the smallest prime factor array for all numbers ≤ limit.
   The array must be of length limit+1 and is filled in place. */
static void build_spf(idx_t limit, uint32_t *spf)
{
    spf[0] = 1;
    spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i)
        spf[i] = 0;                        /* initially unset */

    for (idx_t i = 2; i * i <= limit; ++i)
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (uint32_t)i;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0)
                    spf[j] = (uint32_t)i;
        }

    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0)
            spf[i] = (uint32_t)i;          /* remaining numbers are prime */
}

/* Main public function – returns a(n). */
seq_t a(idx_t n)
{
    if (n == 0)         /* not used, but safe guard */
        return 0;

    seq_t count = 0;    /* the answer */

    /* Allocate smallest‐prime‐factor array */
    uint32_t *spf = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    if (!spf)          /* allocation failed */
        return 0;

    build_spf(n, spf);

    /* Process each possible x */
    for (idx_t x = 2; x <= n; ++x) {
        /* Skip x = 1 (no solutions) and primes (no solutions) */
        if (spf[x] == (uint32_t)x)
            continue;   /* x is prime */

        /* Factorise x once and store exponents */
        int exps[20];
        size_t ecnt = 0;
        idx_t tmp = x;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            exps[ecnt++] = cnt;
        }

        seq_t xplusN = (seq_t)x + (seq_t)n;   /* upper bound for tau(x^y) */

        /* Check each y */
        for (idx_t y = 1; y <= n; ++y) {
            seq_t prod = 1;
            bool bad = false;

            for (size_t k = 0; k < ecnt; ++k) {
                seq_t term = (seq_t)exps[k] * y + 1;   /* y*e_i + 1 */
                if (prod > xplusN / term) {            /* would exceed bound */
                    bad = true;
                    break;
                }
                prod *= term;
                if (prod > xplusN) {                   /* early exit */
                    bad = true;
                    break;
                }
            }

            if (bad)
                continue;

            if (prod == (seq_t)(x + y))   /* satisfied equation */
                count += 1;
        }
    }

    free(spf);
    return count;
}
