/*
 * A023776  Metadromes: digits in base 7 are in strict ascending order.
 * https://oeis.org/A023776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) is the n‑th smallest number whose base‑7 representation has
 *            digits in strictly increasing order
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static cache of all terms (there are only 64 of them) */
static seq_t seq_tbl[64];
static bool seq_inited = false;

/* comparison function for qsort */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* initialise the cache once */
static void init_seq(void)
{
    if (seq_inited) return;
    seq_tbl[0] = (seq_t)0;          /* the single digit 0 term */
    int idx = 1;
    /* all non‑empty subsets of {1,…,6} give a strictly increasing base‑7 number */
    for (int mask = 1; mask < (1 << 6); ++mask) {
        seq_t val = 0;
        for (int d = 0; d < 6; ++d) {
            if (mask & (1 << d)) {
                val = val * (seq_t)7 + (seq_t)(d + 1); /* digits are 1…6 */
            }
        }
        seq_tbl[idx++] = val;
    }
    /* numeric order is required by OEIS; sort the list */
    qsort(seq_tbl, idx, sizeof(seq_t), cmp_seq);
    seq_inited = true;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1 */
    init_seq();
    if (n <= 64) {              /* there are exactly 64 terms */
        return seq_tbl[(unsigned)(n - 1)];
    }
    /* no further terms exist: return 0 (undefined) */
    return (seq_t)0;
}
