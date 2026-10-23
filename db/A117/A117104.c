/*
 * A117104  Sum of two positive heptagonal numbers A000566.
 * https://oeis.org/A117104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Generates all sums of two positive heptagonal numbers
   H(i)+H(j) (with i≤j), sorts them, removes duplicates, and returns the
   n‑th distinct sum. */

/*  H(k) = k * (5*k - 3) / 2  */
#include <stdlib.h>

/* Compare two seq_t values for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t a = *(const seq_t *)p1;
    const seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1 */

    /* Choose a first guess for the maximum index M used to generate
       she sums.  Start with a bound that ensures at least 2*n unordered
       pairs, which is safely larger than the required number of
       distinct sums. */
    idx_t M = 1;
    while ((unsigned __int128)M * (M + 1) / 2 < (unsigned __int128)n * 2)
        ++M;

    /* If the initial bound is not enough (because of duplicates) the
       loop grows M until the desired n‑th term is found. */
    while (1) {
        size_t total_pairs = (size_t)M * (M + 1) / 2;

        /* Heptagonal numbers */
#ifdef __cplusplus
        seq_t *H = new seq_t[M + 1];
#else
        seq_t *H = (seq_t *)malloc((M + 1) * sizeof(seq_t));
#endif
        for (idx_t i = 1; i <= M; ++i) {
            seq_t val = ((seq_t)i * ((seq_t)5 * i - 3)) / 2;
            H[i] = val;
        }

        /* Array of all H(i)+H(j) with i ≤ j */
#ifdef __cplusplus
        seq_t *sums = new seq_t[total_pairs];
#else
        seq_t *sums = (seq_t *)malloc(total_pairs * sizeof(seq_t));
#endif
        size_t pos = 0;
        for (idx_t i = 1; i <= M; ++i) {
            for (idx_t j = i; j <= M; ++j) {
                sums[pos++] = H[i] + H[j];
            }
        }

        /* Sort and deduplicate */
        qsort(sums, total_pairs, sizeof(seq_t), cmp_seq_t);

        idx_t count = 0;
        seq_t prev = 0;
        seq_t result = 0;
        for (size_t k = 0; k < total_pairs; ++k) {
            if (k == 0 || sums[k] != prev) {
                ++count;
                if (count == n) {
                    result = sums[k];
                    break;
                }
                prev = sums[k];
            }
        }

        /* Clean up */
#ifdef __cplusplus
        delete[] H;
        delete[] sums;
#else
        free(H);
        free(sums);
#endif

        if (count >= n)
            return result;

        /* If not enough distinct sums, double M and try again */
        M *= 2;
    }
}
