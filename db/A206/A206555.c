/*
 * A206555  Number of 5's in the last section of the set of partitions of n.
 * https://oeis.org/A206555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} (p(n – 5k) – p(n – 5k – 1)) where p(m) is the
 *            partition number of m (p(0)=1, p(m)=0 for m<0)
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/* Partition number cache (partitions[0..max_n] = p(0)..p(max_n))        */
/* --------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *partitions = NULL;   /* partitions[0] = 1 (p(0)) */
static idx_t part_cap    = 0;      /* allocated size */
static idx_t part_last   = 0;      /* highest index computed (>=0) */

static void ensure_partitions (idx_t upto)
{
    if (partitions == NULL) {
        part_cap = upto + 1;
        partitions = (seq_t *)calloc(part_cap, sizeof(seq_t));
        if (!partitions) abort();
        partitions[0] = (seq_t)1;  /* p(0) */
        part_last = 0;
    } else if (part_cap <= upto) {
        size_t new_cap = part_cap * 2;
        if (new_cap <= upto) new_cap = upto + 1;
        seq_t *tmp = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = part_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        partitions = tmp;
        part_cap = new_cap;
    }

    /* compute new partition values */
    for (idx_t n = part_last + 1; n <= upto; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            seq_t add = partitions[n - g1];
            if ((k % 2) == 0) sum -= add; else sum += add;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > n) continue;
            add = partitions[n - g2];
            if ((k % 2) == 0) sum -= add; else sum += add;
        }
        partitions[n] = sum;
    }
    part_last = upto;
}

/* --------------------------------------------------------------------- */
/* Sequence a(n)                                                             */
/* --------------------------------------------------------------------- */
seq_t a (idx_t n)
{
    if (n < 1) return 0;                 /* below offset – undefined */
    if (n < 5) return 0;                 /* first four terms are 0 */

    /* we need partitions up to n, because the largest index used
       in the sum is n – 5 * 1 = n – 5 */
    ensure_partitions(n);

    seq_t sum = 0;
    for (idx_t m = n - 5; ; ) {          /* m = n – 5k  (k starts at 1) */
        seq_t term = partitions[m];
        if (m >= 1) term -= partitions[m - 1];   /* p(m) – p(m‑1) */
        sum += term;
        if (m < 5) break;                 /* next m would underflow */
        m -= 5;
    }
    return sum;
}
