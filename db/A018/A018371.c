/*
 * A018371  Divisors of 280.
 * https://oeis.org/A018371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th divisor of 280 in ascending order (offset 1) */
#include <stdlib.h>

static seq_t divisors[16];
static int initialized = 0;

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

static void init_divisors(void)
{
    if (initialized) return;

    int idx = 0;
    for (int e0 = 0; e0 <= 3; ++e0)          /* powers of 2 */
    {
        for (int e1 = 0; e1 <= 1; ++e1)      /* powers of 5 */
        {
            for (int e2 = 0; e2 <= 1; ++e2)  /* powers of 7 */
            {
                seq_t val = 1;
                for (int i = 0; i < e0; ++i) val *= 2;
                for (int i = 0; i < e1; ++i) val *= 5;
                for (int i = 0; i < e2; ++i) val *= 7;
                divisors[idx++] = val;
            }
        }
    }
    /* sort ascending */
    qsort(divisors, 16, sizeof(seq_t), cmp_seq);
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */
    init_divisors();
    if (n <= 16) return divisors[n - 1];
    return 0;               /* undefined for n > 16 */
}
