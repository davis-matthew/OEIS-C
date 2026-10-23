/*
 * A185228  Number of disconnected 2-regular simple graphs on n vertices with
 *          girth at least 8.
 * https://oeis.org/A185228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n >= 8) ? (p_{≥8}(n) - 1) : 0 where p_{≥8}(n) is the
 *            number of partitions of n with all parts ≥ 8
 * Range:     u8 n <= 51, u16 n <= 105, u32 n <= 248, u64 n <= 671,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*=== standard includes */
#include <stddef.h>
#include <stdlib.h>

/*=== static cache for the partition table === */
static seq_t *p = NULL;     /* p[i] = #partitions of i using parts ≥ 8 */
static idx_t  max_computed = 0;   /* highest index for which p[.] is valid */

/* recompute partition table up to n (inclusive) */
static void recompute(idx_t n)
{
    /* allocate new table */
    size_t sz = (size_t)(n + 1);
    seq_t *newp = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!newp) return;          /* out of memory – caller will still see old data */

    /* initialise all entries to 0 */
    for (size_t i = 0; i < sz; ++i) newp[i] = 0;
    newp[0] = 1;                 /* empty partition */

    /* dynamic programming: add parts 8 .. n */
    for (idx_t part = 8; part <= n; ++part) {
        for (idx_t s = part; s <= n; ++s) {
            newp[s] += newp[s - part];
        }
    }

    /* replace old table */
    if (p) free(p);
    p = newp;
    max_computed = n;
}

/* ensure that the table is computed up to n */
static void ensure(idx_t n)
{
    if (!p || n > max_computed) recompute(n);
}

/* sequence function */
seq_t a(idx_t n)
{
    ensure(n);
    if (n < 8) return 0;
    seq_t val = p[n];
    val -= 1;   /* remove the single‑part partition */
    return val;
}
