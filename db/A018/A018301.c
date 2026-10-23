/*
 * A018301  Divisors of 140.
 * https://oeis.org/A018301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 140: generate ordered list of all divisors of 140. */
#include <stdlib.h>
#include <stdint.h>

/* Comparison function for qsort on seq_t */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Return the nth divisor of 140 (1‑based).  If n exceeds the number of
   divisors, return 0. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Cache the sorted divisor list of 140. */
    static seq_t divs[256];
    static idx_t div_cnt = 0;

    if (div_cnt == 0) {
        const seq_t N = (seq_t)140;
        trck:
        seq_t x = N;
        for (idx_t i = 1; (seq_t)i * (seq_t)i <= x; ++i) {
            if (x % (seq_t)i == 0) {
                divs[div_cnt++] = (seq_t)i;
                if ((seq_t)i * (seq_t)i != x)
                    divs[div_cnt++] = x / (seq_t)i;
            }
        }
        qsort(divs, div_cnt, sizeof(seq_t), cmp_seq_t);
    }

    if (n <= div_cnt)
        return divs[n - 1];
    else
        return (seq_t)0;
}
