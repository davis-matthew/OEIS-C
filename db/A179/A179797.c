/*
 * A179797  Numbers not of the form abc + abd + acd + bcd, with 0 < a < b < c
 *          < d.
 * https://oeis.org/A179797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 188, u16 n <= 762, u32 n <= 764, u64 n <= 764,
 *            u128 n <= 764, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179797 – Numbers not of the form abc+abd+acd+bcd with 0<a<b<c<d   */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static storage for the precomputation. */
static bool *expressible = NULL;      /* expressible[i] == true ⇔ i is of the form */
static seq_t *non_expr   = NULL;      /* list of non–expressible numbers */
static size_t non_expr_cnt = 0;        /* length of non_expr   */
static const idx_t LIMIT = 200000;     /* covers all listed terms */

/* Build the tables on first use. */
static void init_tables(void)
{
    if (expressible != NULL)
        return; /* already built */

    /* Allocate memory for the boolean array. */
    expressible = (bool *)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (expressible == NULL) {
        return; /* allocation failed – callers will return 0 */
    }

    /* Enumerate all quadruples a<b<c<d with 0<a<b<c<d and mark
       expressible sums.  Early exits are used to keep the loops
       efficient for the chosen LIMIT. */
    for (idx_t a = 1; a <= LIMIT; ++a) {
        for (idx_t b = a + 1; b <= LIMIT; ++b) {
            uint64_t ab = (uint64_t)a * (uint64_t)b;
            /* If abc with the smallest possible c (b+1) exceeds LIMIT, stop.
               ab*(b+1) <= LIMIT  ⇔  ab <= LIMIT/(b+1)  */
            if (ab * (uint64_t)(b + 1) > (uint64_t)LIMIT)
                break;

            for (idx_t c = b + 1; c <= LIMIT; ++c) {
                uint64_t abc = ab * (uint64_t)c;
                if (abc > (uint64_t)LIMIT)
                    break; /* larger c will only increase abc */

                uint64_t coeff = ab
                                + (uint64_t)a * (uint64_t)c
                                + (uint64_t)b * (uint64_t)c;
                /* d starts from c+1 */
                for (idx_t d = c + 1; d <= LIMIT; ++d) {
                    uint64_t sum = abc + (uint64_t)d * coeff;
                    if (sum > (uint64_t)LIMIT)
                        break; /* larger d will only increase sum */
                    expressible[(size_t)sum] = true;
                }
            }
        }
    }

    /* Build the list of non–expressible numbers. */
    non_expr = (seq_t *)malloc((size_t)(LIMIT + 1) * sizeof(seq_t));
    if (non_expr == NULL) {
        return; /* allocation failed – callers will return 0 */
    }

    size_t pos = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (!expressible[(size_t)i]) {
            non_expr[pos++] = (seq_t)i;
        }
    }
    non_expr_cnt = pos;
}

/* Required sequence function. */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0)
        return (seq_t)0; /* undefined for n=0, defensive */

    /* n is 1‑based; array indices are 0‑based. */
    if ((idx_t)(n - 1) < (idx_t)non_expr_cnt) {
        return non_expr[n - 1];
    }

    return (seq_t)0; /* beyond the defined range */
}
