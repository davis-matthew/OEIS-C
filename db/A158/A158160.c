/*
 * A158160  Number of nondecreasing integer sequences of length 26 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{26} Σ_{m=0}^{26-k} p_k(n) * p_m(n) where p_k(n)
 *            is the number of partitions of n into exactly k positive parts.
 *            This equals the number of nondecreasing integer sequences of
 *            length 26 with sum 0 and sum of absolute values 2n
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 44, u64 n <= 166, u128 n <= 259,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define KMAX 26
#define RROWS (KMAX + 1)

/* Table P[n * RROWS + k] = p_k(n) (partitions of n into exactly k parts) */
/* P is lazily grown to accommodate the largest n that has been requested. */
static seq_t *P = NULL;            /* 1‑dimensional flat array */
static idx_t P_max_n = 0;

/* Ensure that P covers 0..target_n, computing new rows as needed. */
static void expand_table(idx_t target_n)
{
    if (target_n <= P_max_n) return;      /* already ready */

    size_t old_size = (size_t)(P_max_n + 1) * RROWS;
    size_t new_size = (size_t)(target_n + 1) * RROWS;

    seq_t *newP = (seq_t *)realloc(P, new_size * sizeof(seq_t));
    if (!newP) abort();   /* out of memory */
    P = newP;

    /* Zero the newly added entries */
    if (new_size > old_size)
        memset((void *)(P + old_size), 0,
               (new_size - old_size) * sizeof(seq_t));

    /* Base case: p_0(0) = 1 */
    if (P_max_n == 0 && P[0] == (seq_t)0) P[0] = (seq_t)1;

    /* Fill rows for n = P_max_n + 1 .. target_n */
    for (idx_t n = P_max_n + 1; n <= target_n; ++n) {
        for (idx_t k = 1; k <= KMAX; ++k) {
            seq_t term1 = (n >= k) ? P[(n - k) * RROWS + k] : (seq_t)0;
            seq_t term2 = P[(n - 1) * RROWS + (k - 1)];
            P[n * RROWS + k] = term1 + term2;
        }
    }

    P_max_n = target_n;
}

/* Main entry point */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined only for n ≥ 1 */

    /* Initialize the table on first call */
    if (P == NULL) {
        P = (seq_t *)calloc(RROWS, sizeof(seq_t));
        if (!P) abort();
        P[0] = (seq_t)1;           /* p_0(0) */
    }

    /* Grow the table as needed for the requested n */
    expand_table(n);

    /* Compute the required double sum */
    idx_t row = n * RROWS;
    seq_t result = (seq_t)0;

    for (idx_t k = 0; k <= KMAX; ++k) {
        seq_t pk = P[row + k];
        if (pk == (seq_t)0) continue;

        /* m can range from 0 to KMAX - k */
        idx_t m_max = KMAX - k;
        for (idx_t m = 0; m <= m_max; ++m) {
            seq_t pm = P[row + m];
            result += pk * pm;
        }
    }

    return result;
}
