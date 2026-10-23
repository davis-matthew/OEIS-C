/*
 * A158178  Number of nondecreasing integer sequences of length 44 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p=1}^{min(44,n)} Σ_{q=1}^{min(44-p,n)} P_p(n)·P_q(n)
 *            where P_k(n) is the number of partitions of n into exactly k
 *            positive parts. The sequence counts nondecreasing integer
 *            sequences of length 44 with sum zero and sum of absolute values
 *            2n
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 133, u128 n <= 199,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_K 44                                 /* maximal number of parts */

#define IDX(n, k) ((n) * (MAX_K + 1) + (k))      /* linear index in the table */

/* table holding P_k(n) for all needed n up to current_max_n */
static seq_t *part_tbl = NULL;        /* array of size (current_max_n+1)*(MAX_K+1) */
static idx_t current_max_n = 0;       /* highest n for which rows have been filled */

/* ------------------------------------------------------------------------ */
/*                    Initialise / grow the partition table                   */
/* ------------------------------------------------------------------------ */
static void init_partitions_upto(idx_t n)
{
    if (n <= current_max_n) return;          /* already available */

    /* compute new size required */
    idx_t new_max_n = n;
    size_t new_size  = (size_t)(new_max_n + 1) * (MAX_K + 1);
    seq_t *new_tbl = NULL;

    if (part_tbl == NULL) {
        /* first allocation */
        new_tbl = (seq_t*)calloc(new_size, sizeof(seq_t));
    } else {
        /* enlarge existing table */
        new_tbl = (seq_t*)realloc(part_tbl, new_size * sizeof(seq_t));
        if (new_tbl) {
            /* zero the newly added rows */
            size_t old_size = (size_t)(current_max_n + 1) * (MAX_K + 1);
            for (size_t i = old_size; i < new_size; ++i)
                new_tbl[i] = 0;
        }
    }
    if (new_tbl == NULL) {
        /* allocation failed – leave the table unchanged */
        return;
    }
    part_tbl = new_tbl;

    /* base case for n==0: P_0(0)=1, others 0 already set by calloc/realloc-zero */
    if (current_max_n == 0) {
        part_tbl[IDX(0,0)] = 1;
    }

    /* compute rows from current_max_n+1 up to n */
    for (idx_t nn = current_max_n + 1; nn <= new_max_n; ++nn) {
        idx_t maxk = (nn < MAX_K) ? nn : MAX_K;
        for (idx_t k = 1; k <= maxk; ++k) {
            seq_t a = part_tbl[IDX(nn - 1, k - 1)];
            seq_t b = (nn >= k) ? part_tbl[IDX(nn - k, k)] : 0;
            part_tbl[IDX(nn, k)] = a + b;
        }
    }
    current_max_n = new_max_n;
}

/* ------------------------------------------------------------------------ */
/*                           Main sequence function                        */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* sequence starts at n = 1 */

    init_partitions_upto(n);
    if (current_max_n < n) return 0;         /* allocation failed */

    idx_t maxk = (n < MAX_K) ? n : MAX_K;
    seq_t result = 0;

    /* prefix sums of P_k(n) for quick range queries */
    seq_t pref[MAX_K + 1];
    pref[0] = 0;                             /* P_0(n)=0 for n>0 */
    for (idx_t k = 1; k <= maxk; ++k) {
        pref[k] = pref[k - 1] + part_tbl[IDX(n, k)];
    }

    for (idx_t p = 1; p <= maxk; ++p) {
        seq_t pp = part_tbl[IDX(n, p)];
        if (!pp) continue;                   /* skip zeros early */
        idx_t limit = (MAX_K - p < maxk) ? (MAX_K - p) : maxk;
        seq_t sum_q = pref[limit];
        result += pp * sum_q;
    }
    return result;
}
