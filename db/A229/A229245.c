/*
 * A229245  Number of set partitions of {1,...,n} with largest set of size 3.
 * https://oeis.org/A229245
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B3[n] - B2[n] where Bk[n] = number of partitions of
 *            {1,…,n} into blocks of size ≤ k. Offsets: 3 (a(3) is the first
 *            term). The recurrence used is Bk[n] = Σ_{j=1..k}
 *            C(n−1,j−1)·Bk[n−j] (Bk[0] = 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *B2 = NULL;          /* Bk ≤2  */
static seq_t *B3 = NULL;          /* Bk ≤3  */
static idx_t  Bsize = 0;          /* current array length */

/* initialise the arrays with index 0 */
static void init_arrays(void)
{
    if (Bsize > 0)
        return;
    B2 = (seq_t*)malloc(sizeof(seq_t));
    B3 = (seq_t*)malloc(sizeof(seq_t));
    if (!B2 || !B3) abort();
    B2[0] = 1;
    B3[0] = 1;
    Bsize = 1;
}

/* extend the arrays so that indices 0…n are defined */
static void compute_up_to(idx_t n)
{
    if (n + 1 <= Bsize)
        return;

    B2 = (seq_t*)realloc(B2, (n + 1) * sizeof(seq_t));
    B3 = (seq_t*)realloc(B3, (n + 1) * sizeof(seq_t));
    if (!B2 || !B3) abort();

    for (idx_t i = Bsize; i <= n; ++i) {
        /* B2 recurrence: j = 1,2 */
        seq_t sum2 = 0;
        if (i >= 1) sum2 += B2[i - 1];
        if (i >= 2) sum2 += (seq_t)(i - 1) * B2[i - 2];
        B2[i] = sum2;

        /* B3 recurrence: j = 1,2,3 */
        seq_t sum3 = 0;
        if (i >= 1) sum3 += B3[i - 1];
        if (i >= 2) sum3 += (seq_t)(i - 1) * B3[i - 2];
        if (i >= 3)
            sum3 += ((seq_t)(i - 1) * (i - 2) / 2) * B3[i - 3];
        B3[i] = sum3;
    }
    Bsize = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;         /* undefined below the offset */
    init_arrays();
    compute_up_to(n);
    return B3[n] - B2[n];
}
