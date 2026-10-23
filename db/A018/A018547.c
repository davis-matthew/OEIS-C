/*
 * A018547  Divisors of 608.
 * https://oeis.org/A018547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 608 in ascending order
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t s_divisors[20];   /* enough space for all 12 divisors */
static size_t s_divcnt = 0;    /* number of divisors stored */
static int s_initialized = 0;  /* flag for one‑time initialisation */

/* Compare two seq_t values for qsort */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Pre‑compute all divisors of 608 (once) */
static void init_divisors(void)
{
    if (s_initialized) return;
    s_initialized = 1;

    const idx_t LIMIT = (idx_t)608;          /* the number to factor */
    for (idx_t i = (idx_t)1; i * i <= LIMIT; ++i) {
        if (LIMIT % i == (idx_t)0) {
            s_divisors[s_divcnt++] = (seq_t)i;
            if (i != LIMIT / i) {
                s_divisors[s_divcnt++] = (seq_t)(LIMIT / i);
            }
        }
    }
    qsort(s_divisors, s_divcnt, sizeof(seq_t), cmp_seq);
}

/* Return the n‑th divisor of 608 */
seq_t a(idx_t n)
{
    init_divisors();

    if (n >= (idx_t)1 && n <= (idx_t)s_divcnt)
        return s_divisors[n - 1];

    /* Undefined for indices outside the finite sequence */
    return (seq_t)0;
}
