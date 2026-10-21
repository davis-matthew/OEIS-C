/*
 * A083690  a(n) = number of partitions of n wherein the sum of the 1's is no
 *          more than the sum of the other parts.
 * https://oeis.org/A083690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - p(floor((n-1)/2)) where p(k) is the partition
 *            function
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --------------  partitions table  ---------------- */
static seq_t *p_tbl = NULL;   /* p_tbl[0..max_computed] holds p(i).  */
static idx_t alloc_len = 0;   /* allocated length of p_tbl (indices 0..alloc_len).  */
static idx_t max_computed = 0;/* largest n for which p_tbl[n] is valid. */

// Forward declaration of helper to rebuild the table to requested size.
static void ensure_partitions(idx_t N);

/*
    Compute the OEIS sequence A083690.
    Valid for all n >= 1; behaviour for n < 1 is undefined.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at 1; 0 is undefined */

    /* common index of the second partition */
    idx_t m = (n - 1) / 2;        /* floor((n-1)/2) */

    /* compute partitions up to n and m (if needed) */
    ensure_partitions(n);
    if (m > max_computed)
        ensure_partitions(m);

    return p_tbl[n] - p_tbl[m];
}

/*
    Ensure that the partition table contains values p(0) .. p(N).
    Uses Euler's pentagonal number theorem.
    Paritions are cached so that repeated calls are O(1).
 */
static void ensure_partitions(idx_t N)
{
    if (N < 1)
        return;                   /* nothing to do for N < 1 */

    /* allocate or grow the table */
    if (alloc_len <= N) {
        idx_t new_len = alloc_len ? alloc_len << 1 : 128;
        while (new_len <= N)
            new_len <<= 1;

        /* realloc returns void*; the C++ compiler requires a cast */
        seq_t *tmp = (seq_t *)realloc(p_tbl, (new_len + 1) * sizeof(seq_t));
        if (!tmp)
            abort();               /* allocation failure */
        p_tbl = tmp;

        /* zero‑initialise the newly allocated slots */
        for (idx_t i = alloc_len + 1; i <= new_len; ++i)
            p_tbl[i] = 0;

        alloc_len = new_len;
    }

    /* initialise p(0) once */
    if (max_computed == 0) {
        p_tbl[0] = 1;               /* p(0) = 1 */
        max_computed = 0;
    }

    /* compute new partition values from max_computed+1 up to N */
    for (idx_t n = max_computed + 1; n <= N; ++n) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = (k * (3 * k - 1)) / 2;
            if (pent1 > n)
                break;
            seq_t term = p_tbl[n - pent1];
            if (k & 1)
                pos += term;
            else
                neg += term;

            idx_t pent2 = (k * (3 * k + 1)) / 2;
            if (pent2 > n)
                continue;
            term = p_tbl[n - pent2];
            if (k & 1)
                pos += term;
            else
                neg += term;
        }
        p_tbl[n] = pos - neg;            /* guaranteed non‑negative */
    }
    max_computed = N;
}
