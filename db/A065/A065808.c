/*
 * A065808  Square of n has a smaller square as its final digits.
 * https://oeis.org/A065808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th integer k such that k² ends with the decimal
 *            representation of a smaller perfect square
 * Range:     u8 n <= 8, u16 n <= 243, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/*---------------------------------------------------------------*/
/*  Pre‑computation of small perfect squares                      */
/*---------------------------------------------------------------*/
#define MAX_L 12                     /* consider divisor 10^MAX_L           */
static seq_t *sqs[MAX_L + 1];          /* arrays of integers m², 0 ≤ m < 10^l  */
static size_t sqs_sizes[MAX_L + 1];    /* number of elements in each array    */
static seq_t pow10[MAX_L + 1];         /* 10^l values                         */
static int squares_prepared = 0;

/* Generates all squares < 10^l for l = 1..MAX_L                    */
static void prepare_squares(void)
{
    if (squares_prepared) return;
    squares_prepared = 1;

    /* 10^l */
    pow10[0] = 1;
    for (int i = 1; i <= MAX_L; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;

    /* compute squares up to each 10^l */
    for (int l = 1; l <= MAX_L; ++l) {
        seq_t limit = pow10[l];
        /* number of squares < limit  ==  floor(sqrt(limit-1)) + 1   */
        seq_t m = 0;
        while (m * m < limit) ++m;
        sqs_sizes[l] = (size_t)m;                      /* m values from 0 to m-1 */
        sqs[l] = (seq_t *)malloc(sqs_sizes[l] * sizeof(seq_t));
        for (size_t i = 0; i < sqs_sizes[l]; ++i)
            sqs[l][i] = (seq_t)i * (seq_t)i;
    }
}

/* Binary search for tail in sqs[l]  (m reports index of square)       */
static int check_tail_in_squares(seq_t tail, int l, seq_t candidate)
{
    size_t left = 0, right = sqs_sizes[l];
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if (sqs[l][mid] < tail)
            left = mid + 1;
        else
            right = mid;
    }
    if (left < sqs_sizes[l] && sqs[l][left] == tail) {
        seq_t m = (seq_t)left;                 /* m² = tail                         */
        if (m < candidate)                     /* smaller square required           */
            return 1;
    }
    return 0;
}

/* Checks whether n satisfies the required property                 */
static int has_property(seq_t n)
{
    prepare_squares();                     /* ensure tables built once           */
    seq_t n_sq = n * n;

    seq_t divisor = (seq_t)10;             /* 10^1  */
    int l = 1;
    while (divisor <= n_sq) {
        seq_t tail = n_sq % divisor;
        if (check_tail_in_squares(tail, l, n))
            return 1;
        /* Avoid overflow when multiplying divisor by 10 */
        if (divisor > n_sq / (seq_t)10) break;
        divisor *= (seq_t)10;
        ++l;
        if (l > MAX_L) break;              /* safety guard                       */
    }
    return 0;
}

/*---------------------------------------------------------------*/
/*  Caching of sequence terms                                    */
/*---------------------------------------------------------------*/
static seq_t *cache = NULL;                /* holds found terms                 */
static size_t cache_size = 0;              /* allocated capacity of cache[]     */
static idx_t term_count = 0;               /* number of terms stored in cache[] */
static idx_t next_candidate = 1;           /* next integer to test             */

/*---------------------------------------------------------------*/
/*  Main function required by the problem                        */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* defined safely */

    while ((idx_t)term_count < n) {                 /* generate until enough terms */
        seq_t cand = (seq_t)next_candidate;
        if (has_property(cand)) {
            if ((size_t)term_count == cache_size) {
                cache_size = (cache_size == 0) ? 16 : cache_size * 2;
                cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
            }
            cache[term_count] = cand;
            ++term_count;
        }
        ++next_candidate;
    }

    return cache[n - 1];                            /* 1‑based indexing               */
}
