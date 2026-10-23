/*
 * A035696  Number of partitions of n into parts 8k+4 and 8k+7 with at least
 *          one part of each type.
 * https://oeis.org/A035696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 200, u32 n <= 553, u64 n <= 1735,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A035696: Number of partitions of n into parts 8k+4 and 8k+7
 *  with at least one part of each type.
 *  The sequence is computed by dynamic programming:
 *  Let p4[i] be the number of partitions of i using only parts of the form 8k+4,
 *  and p7[i] the number using only parts of the form 8k+7.
 *  Then the desired value is
 *      a(n) = Σ_{m=0}^{n} p4[m] * p7[n-m]  -  p4[n]  -  p7[n]
 *  (subtracting the cases that use only one type of part).
 *  The tables p4, p7 and a are generated lazily and kept in static storage
 *  so that subsequent calls can reuse the results.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External types defined by the build system */
typedef idx_t idx_t;
typedef seq_t seq_t;

static seq_t *p4 = NULL;
static seq_t *p7 = NULL;
static seq_t *a_arr = NULL;
static idx_t maxN = 0;

/* Allocate and initialize a table of seq_t values up to n */
static void init_tables(idx_t n)
{
    /* Allocate new tables */
    seq_t *new_p4 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *new_p7 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *new_a  = (seq_t *)malloc((n + 1) * sizeof(seq_t));

    if (!new_p4 || !new_p7 || !new_a) {
        /* Allocation failed – keep old tables (should not happen in tests) */
        return;
    }

    /* Free old tables */
    free(p4);
    free(p7);
    free(a_arr);

    p4 = new_p4;
    p7 = new_p7;
    a_arr = new_a;

    /* Initialize p4[0] = p7[0] = 1 (empty partition) */
    p4[0] = (seq_t)1;
    p7[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        p4[i] = 0;
        p7[i] = 0;
    }

    /* Compute partitions using parts 8k+4 */
    for (idx_t part = 4; part <= n; part += 8) {
        for (idx_t i = part; i <= n; ++i) {
            p4[i] += p4[i - part];
        }
    }

    /* Compute partitions using parts 8k+7 */
    for (idx_t part = 7; part <= n; part += 8) {
        for (idx_t i = part; i <= n; ++i) {
            p7[i] += p7[i - part];
        }
    }

    /* Compute a[n] using convolution minus single-type counts */
    a_arr[0] = 0;   /* unused */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t m = 0; m <= i; ++m) {
            sum += p4[m] * p7[i - m];
        }
        sum -= p4[i];
        sum -= p7[i];
        a_arr[i] = sum;
    }

    maxN = n;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* Sequence is defined for n >= 1, but return 0 for safety */
    }

    if (n > maxN) {
        init_tables(n);
    }

    return a_arr[n];
}
