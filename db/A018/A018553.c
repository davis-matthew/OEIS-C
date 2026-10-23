/*
 * A018553  Divisors of 618.
 * https://oeis.org/A018553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018553: Divisors of 618.  a(n) is the n‑th smallest divisor of 618 (offset 1). */

#include <stdlib.h>
#include <stdbool.h>

/* Compare function for qsort on seq_t */
static int cmp_seq(const void *p, const void *q) {
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Holds the sorted divisors of 618 */
static seq_t divs[16];
static size_t div_cnt = 0;
static bool divs_init = false;

static void init_divs(void) {
    if (divs_init)
        return;
    const idx_t N = 618;               /* The number whose divisors are needed   */
    size_t cnt = 0;
    for (idx_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            divs[cnt++] = (seq_t)i;
            if (i != N / i)
                divs[cnt++] = (seq_t)(N / i);
        }
    }
    /* Sort the divisors in ascending order */
    qsort(divs, cnt, sizeof(seq_t), cmp_seq);
    div_cnt = cnt;
    divs_init = true;
}

seq_t a(idx_t n) {
    init_divs();
    if (n == 0 || n > div_cnt)
        return (seq_t)0;           /* out of bounds → 0 (typical OEIS convention) */
    return divs[n - 1];
}
