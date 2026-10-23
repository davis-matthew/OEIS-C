/*
 * A018328  Divisors of 192.
 * https://oeis.org/A018328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth smallest divisor of 192 (1‑indexed)
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Comparison function for qsort that works with any unsigned seq_t type */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Cache of the 14 divisors of 192, constructed lazily */
static seq_t divs[14];
static bool initialized = false;

/* Build the divisors of 192 = 2^6 * 3^1 and sort them */
static void init_divs(void)
{
    idx_t pos = 0;
    for (idx_t i = 0; i <= 6; ++i) {
        seq_t base = (seq_t)1;
        base <<= i;                /* 2^i */
        for (idx_t j = 0; j <= 1; ++j) {
            seq_t d = base;
            if (j) d *= (seq_t)3;  /* multiply by 3 if j == 1 */
            divs[pos++] = d;
        }
    }
    /* pos should be 14 here */
    qsort(divs, 14, sizeof(divs[0]), cmp_seq_t);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_divs();
    if (n < 1 || n > 14) return (seq_t)0;  /* undefined beyond the 14 divisors */
    return divs[n - 1];
}
