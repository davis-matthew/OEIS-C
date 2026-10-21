/*
 * A087184  Partition numbers of the form 3*k+1.
 * https://oeis.org/A087184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(A237276(n)) Computes the partition numbers p(m)
 *            where m is the n‑th integer for which p(m) ≡ 1 (mod 3). The
 *            implementation caches the partition sequence and the qualifying
 *            indices to answer any a(n) in O(1) after at most a linear
 *            precomputation in the largest required index
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 40, u64 n <= 127, u128 n <= 487,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/*  seq_t and idx_t are defined by the build system. */

/* ----------  dynamic arrays for partitions and qualifiers  ---------- */

static seq_t *partitions = NULL;      /* partitions[0] = p(0) = 1        */
static size_t part_len  = 0;          /* number of stored values         */
static size_t part_cap  = 0;          /* allocated capacity              */

static idx_t *qualifying = NULL;      /* indices m with p(m) ≡ 1 (mod 3) */
static size_t qual_len  = 0;
static size_t qual_cap  = 0;

/* append a value to the partitions array, growing it if necessary */
static void append_partition(seq_t val)
{
    if (part_len == part_cap) {
        size_t new_cap = part_cap ? part_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
        assert(tmp != NULL);  /* assume realloc never fails in tests */
        partitions = tmp;
        part_cap  = new_cap;
    }
    partitions[part_len++] = val;
}

/* append an index to the qualifying array, growing it if necessary */
static void append_qualifying(idx_t val)
{
    if (qual_len == qual_cap) {
        size_t new_cap = qual_cap ? qual_cap * 2 : 4;
        idx_t *tmp = (idx_t *)realloc(qualifying, new_cap * sizeof(idx_t));
        assert(tmp != NULL);
        qualifying = tmp;
        qual_cap   = new_cap;
    }
    qualifying[qual_len++] = val;
}

/* ----------  ensure partitions up to at least target_k (inclusive)  ---------- */

static void ensure_partitions(idx_t target_k)
{
    /* initialise if necessary */
    if (part_len == 0) {
        append_partition((seq_t)1);          /* p(0) = 1 */
        append_qualifying((idx_t)0);         /* 0 satisfies p(0) ≡ 1 (mod 3) */
    }

    while (part_len <= target_k) {
        idx_t n = part_len;                 /* compute p(n)  */
        seq_t sum = 0;

        /* Euler's pentagonal number recurrence */
        for (idx_t m = 1; ; ++m) {
            idx_t g1 = m * (3*m - 1) / 2;
            if (g1 > n) break;
            if ((m & 1) != 0)
                sum += partitions[n - g1];
            else
                sum -= partitions[n - g1];

            idx_t g2 = m * (3*m + 1) / 2;
            if (g2 > n) continue;
            if ((m & 1) != 0)
                sum += partitions[n - g2];
            else
                sum -= partitions[n - g2];
        }

        append_partition(sum);

        if ((sum % (seq_t)3) == (seq_t)1)
            append_qualifying(n);
    }
}

/* ----------  ensure that the nth qualifying index exists  ---------- */

static void ensure_sequence(idx_t n_query)
{
    /* n_query counts from 1 */
    while (qual_len < n_query) {
        /* expand search range: double the current number of computed partitions */
        idx_t new_target = part_len ? part_len * 2 : 1;
        ensure_partitions(new_target);
    }
}

/* ----------  the required OEIS entry function  ---------- */

seq_t a(idx_t n)
{
    assert(n > 0);                       /* offset 1, values for n ≥ 1 only */

    ensure_sequence(n);

    idx_t k = qualifying[n - 1];          /* 0‑based indexing of qualifiers */
    return partitions[k];
}
