/*
 * A105488  Number of partitions of {1...n} containing 2 detached pairs of
 *          consecutive integers, i.e., partitions in which only 1- or
 *          2-strings of consecutive integers can appear in a block and there
 *          are exactly two 2-strings.
 * https://oeis.org/A105488
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n-2, 2) * Bell(n-3)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 4  (a(4) = 1) */

#include <stdlib.h>   /* for malloc, realloc */

/* Dynamic array of Bell numbers: bell[i] = Bell(i) */
static seq_t *bell = NULL;   /* array of Bell numbers */
static idx_t bell_size = 0;  /* number of Bell numbers stored */
static idx_t bell_cap  = 0;  /* allocated capacity */

/* Ensure that Bell(k) is computed for all k <= needed */
static void ensureBell(idx_t needed)
{
    /* initialise the array if it has not been allocated yet */
    if (bell_cap == 0) {
        bell_cap = 1;
        bell = (seq_t *)malloc(bell_cap * sizeof(seq_t));
        if (!bell) return;                /* allocation failed – leave NULL */
        bell[0] = (seq_t)1;               /* Bell(0) = 1 */
        bell_size = 1;
    }

    /* extend the array until it contains Bell(needed) */
    while (bell_size <= needed) {
        /* grow storage if necessary */
        if (bell_cap <= bell_size) {
            bell_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(bell, bell_cap * sizeof(seq_t));
            if (!tmp) return;             /* allocation error – abort */
            bell = tmp;
        }

        /* compute Bell(bell_size) where bell_size is the next index */
        idx_t m = bell_size;                /* we need Bell(m) */
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;             /* binom = C(m-1,0) */

        for (idx_t s = 0; s < m; ++s) {
            /* sum += binom * Bell(s) */
            sum += binom * bell[s];

            /* update binom for next s: C(m-1, s+1) = C(m-1, s) * (m-1-s)/(s+1) */
            if (s + 1 < m) {
                binom = binom * (m - 1 - s);
                binom = binom / (s + 1);
            }
        }

        /* store the new Bell number */
        bell[bell_size] = sum;
        ++bell_size;
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 4; for n < 4 return 0 */
    if (n < 4)
        return (seq_t)0;

    /* Compute Bell(n-3) if necessary */
    idx_t k = n - 3;
    ensureBell(k);

    /* Compute binomial(n-2, 2) */
    seq_t comb = ((seq_t)(n - 2) * (seq_t)(n - 3)) / 2U;

    /* Return the product */
    return comb * bell[k];
}
