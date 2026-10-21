/*
 * A229246  Number of set partitions of {1,...,n} with largest set of size 4.
 * https://oeis.org/A229246
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=B4(n)-B3(n) where Bk(0)=1 and Bk(n)=∑_{j=1..k} C(n-1,
 *            j-1)*Bk(n-j). The function returns the number of set partitions
 *            of {1,…,n} with largest block of size exactly 4. (Offset 4.)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *B4 = NULL;       /* partitions with block size ≤4 */
static seq_t *B3 = NULL;       /* partitions with block size ≤3 */
static idx_t   computed = 0;   /* highest n for which B4,B3 are filled */

/* Extend tables up to index n (inclusive). */
static void ensure_to(idx_t n)
{
    if (n <= computed)
        return;

    size_t new_cap = (size_t)(n + 1);

    /* Allocate or grow the arrays. */
    if (B4 == NULL) {
        B4 = (seq_t *)malloc(new_cap * sizeof(seq_t));
        B3 = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (B4 == NULL || B3 == NULL)
            exit(EXIT_FAILURE);
        B4[0] = 1;
        B3[0] = 1;
    } else {
        seq_t *tmp = (seq_t *)realloc(B4, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            exit(EXIT_FAILURE);
        B4 = tmp;
        tmp = (seq_t *)realloc(B3, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            exit(EXIT_FAILURE);
        B3 = tmp;
    }

    /* Fill entries from previously computed+1 up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum4 = 0, sum3 = 0;

        /* Pre‑compute the needed binomial coefficients for level i. */
        seq_t binom1 = (seq_t)(i - 1);          /* C(i-1,1) */
        seq_t binom2 = 0, binom3 = 0;
        if (i >= 3)
            binom2 = (seq_t)(i - 1) * (seq_t)(i - 2) / 2;          /* C(i-1,2) */
        if (i >= 4)
            binom3 = (seq_t)(i - 1) * (seq_t)(i - 2) *
                     (seq_t)(i - 3) / 6;                              /* C(i-1,3) */

        /* B4 recurrence: j = 1..4 */
        sum4 += B4[i - 1];                      /* j=1, C=1 */
        if (i >= 2) sum4 += binom1 * B4[i - 2]; /* j=2 */
        if (i >= 3) sum4 += binom2 * B4[i - 3]; /* j=3 */
        if (i >= 4) sum4 += binom3 * B4[i - 4]; /* j=4 */

        /* B3 recurrence: j = 1..3 */
        sum3 += B3[i - 1];                      /* j=1, C=1 */
        if (i >= 2) sum3 += binom1 * B3[i - 2]; /* j=2 */
        if (i >= 3) sum3 += binom2 * B3[i - 3]; /* j=3 */

        B4[i] = sum4;
        B3[i] = sum3;
    }

    computed = n;
}

/* External function requested. */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                    /* undefined for n<4, safe fallback */
    ensure_to(n);
    return B4[n] - B3[n];
}
