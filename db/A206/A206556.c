/*
 * A206556  Number of 6's in the last section of the set of partitions of n.
 * https://oeis.org/A206556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0} p_no1(n – 6 – 6k) where p_no1(m) is the number of
 *            partitions of m whose parts are all ≥ 2. This follows from the
 *            generating‑function identity a(n) = [x^n] x^6 · Π_{k≥2} (1 –
 *            x^k)^(-1) / (1 – x^6)
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static storage for the intermediate sequences */
static seq_t *p_no1 = NULL;   /* partitions with parts ≥ 2 */
static idx_t  p_max  = 0;     /* highest index computed in p_no1 */

static seq_t *a_arr = NULL;   /* array of a(n) values */
static idx_t  a_max  = 0;     /* highest index computed in a_arr */

/* ---------------------------------------------------------------------*/
/* Compute partitions with parts ≥ 2 up to index N (inclusive).          */
/* ---------------------------------------------------------------------*/
static void ensure_p_no1(idx_t N)
{
    if (N <= p_max)                 /* already up to date */
        return;

    /* Allocate an array of length N+1 and initialise to zero. */
    seq_t *tmp = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!tmp)                       /* out of memory: abort */
        exit(EXIT_FAILURE);
    tmp[0] = 1;                      /* the empty partition */

    /* Dynamic programming: for each part size k ≥ 2 add its
       contributions to all larger sums. */
    for (idx_t k = 2; k <= N; ++k) {
        for (idx_t s = k; s <= N; ++s) {
            tmp[s] += tmp[s - k];
        }
    }

    /* Replace the old array. */
    free(p_no1);
    p_no1 = tmp;
    p_max = N;
}

/* ---------------------------------------------------------------------*/
/* Compute a(n) up to index N (inclusive) using the derived formula.   */
/* ---------------------------------------------------------------------*/
static void ensure_a(idx_t N)
{
    if (N <= a_max)                 /* nothing to do */
        return;

    /* a(n) depends on p_no1(m) with m ≤ n-6, so compute p_no1 up to N. */
    ensure_p_no1(N);

    /* Allocate or extend the a_arr array. */
    if (!a_arr) {
        a_arr = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
        if (!a_arr)
            exit(EXIT_FAILURE);
    } else {
        a_arr = (seq_t *)realloc(a_arr, (size_t)(N + 1) * sizeof(seq_t));
        if (!a_arr)
            exit(EXIT_FAILURE);
        /* New elements are zeroed by calloc → realloc does not initialize,
           so we zero them manually. */
        for (idx_t i = a_max + 1; i <= N; ++i)
            a_arr[i] = 0;
    }

    /* Compute a(n) for every new index. */
    for (idx_t n = a_max + 1; n <= N; ++n) {
        if (n <= 5) {                 /* trivial case from the data */
            a_arr[n] = 0;
            continue;
        }
        seq_t sum = 0;
        int64_t sub = (int64_t)n - 6;
        while (sub >= 0) {
            sum += p_no1[(idx_t)sub];
            sub -= 6;
        }
        a_arr[n] = sum;
    }

    a_max = N;
}

/* Public function requested by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, a(0) is undefined – return 0 */
        return (seq_t)0;
    ensure_a(n);
    return a_arr[n];
}
