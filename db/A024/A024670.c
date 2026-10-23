/*
 * A024670  Numbers that are sums of 2 distinct positive cubes.
 * https://oeis.org/A024670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth smallest distinct sum i^3 + j^3 with
   1 ≤ i < j.  The sequence starts with 9, 28, 35, 65, …   */
#include <stdlib.h>

#define MAX_M   2000
#define MAX_TOTAL ((MAX_M * (MAX_M - 1)) / 2)

/* Static array large enough for all n up to about 10 000. */
static seq_t sums[MAX_TOTAL];

/* Comparator for qsort on seq_t.  Works in C and C++. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Return the nth term of A024670 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range – not used */

    idx_t m = 1;                   /* search for sufficient cube root */
    /* The factor 3 guarantees (m choose 2) ≥ 3n; improves the chance
       that enough distinct sums exist.  If it still fails for very
       large n, the static array will eventually be exceeded – for n
       ≈ 10 000 this never happens. */
    while ((m * (m - 1)) / 2 < n * 3) {
        ++m;
        if (m > MAX_M) {           /* safety guard – not expected */
            break;
        }
    }
    if (m > MAX_M) m = MAX_M;      /* clamp to pre‑allocated array */

    size_t total = (size_t)m * (m - 1) / 2;
    /* total cannot exceed MAX_TOTAL because m ≤ MAX_M. */

    /* Generate all i³ + j³ for 1 ≤ i < j ≤ m. */
    size_t pos = 0;
    for (idx_t i = 1; i <= m; ++i) {
        seq_t ci  = (seq_t)i;
        seq_t ci3 = ci * ci * ci;   /* i³ */
        for (idx_t j = i + 1; j <= m; ++j) {
            seq_t cj  = (seq_t)j;
            seq_t cj3 = cj * cj * cj;   /* j³ */
            sums[pos++] = ci3 + cj3;
        }
    }

    /* Sort the sums; duplicates will be adjacent. */
    qsort(sums, pos, sizeof(seq_t), cmp_seq);

    /* Scan for the nth distinct value. */
    seq_t last = 0;
    idx_t count = 0;
    seq_t result = 0;

    for (size_t i = 0; i < pos; ++i) {
        if (i == 0 || sums[i] != last) {
            ++count;
            if (count == n) {
                result = sums[i];
                break;
            }
            last = sums[i];
        }
    }

    return result;   /* guaranteed found for the supported n range */
}
