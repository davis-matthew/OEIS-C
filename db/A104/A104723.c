/*
 * A104723  Moebius transform of Jacobsthal numbers.
 * https://oeis.org/A104723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d)*J(d) (J = Jacobsthal numbers)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mobius.h"

/* static Jacobsthal table and its current length (number of stored terms) */
static seq_t *jac = NULL;
static idx_t jac_len = 0;

/* extend Jacobsthal table up to index n (inclusive) */
static void extend_jacobsthal(idx_t n)
{
    if (n + 1 <= jac_len)
        return;                                 /* already computed up to n */

    size_t new_len = (size_t)n + 1;
    seq_t *new_tab = (seq_t *)realloc(jac, new_len * sizeof(seq_t));
    if (!new_tab)
        return;                                 /* allocation failed – keep old table */

    /* initialise first two terms if table was empty */
    if (jac_len == 0) {
        new_tab[0] = 0;
        if (new_len > 1)
            new_tab[1] = 1;
    } else if (jac_len == 1 && new_len > 1) {
        new_tab[1] = 1;
    }

    /* compute new entries */
    idx_t start = jac_len > 1 ? jac_len : 2;
    for (idx_t i = start; i <= n; ++i) {
        new_tab[i] = new_tab[i - 1] + (seq_t)2 * new_tab[i - 2];
    }

    jac = new_tab;
    jac_len = new_len;
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset 0 */

    extend_jacobsthal(n);

    seq_t pos = 0, neg = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            int mu1 = mobius(d2);   /* μ(n/d1) = μ(d2) */
            if (mu1 == 1)
                pos += jac[d1];
            else if (mu1 == -1)
                neg += jac[d1];

            if (d1 != d2) {
                int mu2 = mobius(d1);   /* μ(n/d2) = μ(d1) */
                if (mu2 == 1)
                    pos += jac[d2];
                else if (mu2 == -1)
                    neg += jac[d2];
            }
        }
    }

    return pos - neg;                  /* pos ≥ neg for all n ≥ 0 */
}
