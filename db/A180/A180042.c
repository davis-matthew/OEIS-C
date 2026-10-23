/*
 * A180042  Possible orders of cyclic groups that can be realized as holonomy
 *          groups of crystallographic groups in dimension 7.
 * https://oeis.org/A180042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth smallest integer m such that * the sum of Euler's
 *            totient values of the prime‑power * components of m is ≤ 7
 *            (i.e., ∑_{p^k||m} φ(p^k) ≤ 7 )
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* comparison function for qsort on seq_t */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t x = *(const seq_t *)p1;
    seq_t y = *(const seq_t *)p2;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static seq_t *seq_vals = NULL;
static size_t seq_len = 0;

/* Build the sorted list of all integers m with
 * sum of φ(p^k) over the prime‑power factors ≤ 7 */
static void build_seq(void)
{
    if (seq_vals) return;               /* already built */

    /* Prime power candidates with φ ≤ 7 */
    const struct { seq_t val, phi; } p2[4] = { {1,0}, {2,1}, {4,2}, {8,4} };
    const struct { seq_t val, phi; } p3[3] = { {1,0}, {3,2}, {9,6} };
    const struct { seq_t val, phi; } p5[2] = { {1,0}, {5,4} };
    const struct { seq_t val, phi; } p7[2] = { {1,0}, {7,6} };

    /* Allocate capacity for all possible combinations (≤ 4·3·2·2 = 48) */
    seq_vals = (seq_t *)malloc(64 * sizeof(seq_t));
    if (!seq_vals) return;              /* out of memory – should not happen */

    size_t idx = 0;
    for (size_t i = 0; i < sizeof(p2)/sizeof(p2[0]); ++i)
    for (size_t j = 0; j < sizeof(p3)/sizeof(p3[0]); ++j)
    for (size_t k = 0; k < sizeof(p5)/sizeof(p5[0]); ++k)
    for (size_t l = 0; l < sizeof(p7)/sizeof(p7[0]); ++l)
    {
        seq_t sumphi = p2[i].phi + p3[j].phi + p5[k].phi + p7[l].phi;
        if (sumphi <= 7)
        {
            seq_t val = p2[i].val * p3[j].val * p5[k].val * p7[l].val;
            seq_vals[idx++] = val;
        }
    }

    seq_len = idx;
    /* sort and remove duplicates (duplicates cannot occur here, but keep safe) */
    qsort(seq_vals, seq_len, sizeof(seq_t), cmp_seq);

    /* ensure unique values if any duplicates appear */
    size_t uniq = 1;
    for (size_t i = 1; i < seq_len; ++i)
        if (seq_vals[i] != seq_vals[uniq-1])
            seq_vals[uniq++] = seq_vals[i];
    seq_len = uniq;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n < 1 */
    build_seq();
    size_t i = (size_t)n - 1;
    if (i < seq_len) return seq_vals[i];
    return 0;                         /* beyond the defined sequence */
}
