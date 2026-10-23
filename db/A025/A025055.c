/*
 * A025055  Positions of odd numbers in sequence A025051 (numbers of form j*k
 *          + k*i + i*j, without repetitions, where 1 <= i <= j <= k).
 * https://oeis.org/A025055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th odd number among the distinct values
 *            * S = i*j + j*k + i*k with 1 <= i <= j <= k, sorted
 *            increasingly. * The code builds the set of all such values up to
 *            i,j,k <= 300, * removes duplicates, then records the indices of
 *            the odd ones. * a(1) = 1, a(2) = 2, ... as required
 * Range:     u8 none, u16 none, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global state for the one-time precomputation. */
static int _a025055_initialized = 0;

static seq_t *_a025055_solutions = NULL; /* sorted unique values S */
static idx_t   _a025055_solutions_count = 0;

static idx_t  *_a025055_odd_positions = NULL; /* 1‑based positions of odd S */
static idx_t   _a025055_odd_count = 0;

/* Comparator for qsort; works for both integral seq_t and bigint classes. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Performs the one-time precomputation. */
static void _a025055_init(void)
{
    if (_a025055_initialized)
        return;

    /* Upper bound for i,j,k to produce many odd entries (300 is ample). */
    const idx_t I_MAX = 300;

    /* Number of triples i≤j≤k with i,j,k ≤ I_MAX:
     * C(I_MAX+2, 3) = (I_MAX+2)*(I_MAX+1)*I_MAX / 6
     */
    const idx_t triple_count = (I_MAX + 2) * (I_MAX + 1) * I_MAX / 6;

    /* Allocate temporary array for all values. */
    seq_t *all_vals = (seq_t *)malloc((size_t)triple_count * sizeof(seq_t));
    if (!all_vals) return; /* allocation failed – unlikely here */

    /* Generate all values S = i*j + j*k + i*k. */
    idx_t idx = 0;
    for (idx_t i = 1; i <= I_MAX; ++i) {
        for (idx_t j = i; j <= I_MAX; ++j) {
            seq_t ij = (seq_t)i * (seq_t)j;
            for (idx_t k = j; k <= I_MAX; ++k) {
                seq_t val = ij + (seq_t)i * (seq_t)k + (seq_t)j * (seq_t)k;
                all_vals[idx++] = val;
            }
        }
    }

    /* Sort the array. */
    qsort(all_vals, (size_t)triple_count, sizeof(seq_t), cmp_seq_t);

    /* Remove duplicates into _a025055_solutions. */
    _a025055_solutions_count = 0;
    _a025055_solutions = (seq_t *)malloc((size_t)triple_count * sizeof(seq_t));
    if (!_a025055_solutions) {
        free(all_vals);
        return;
    }

    seq_t last = 0;
    int first = 1;
    for (idx_t i = 0; i < triple_count; ++i) {
        if (first || all_vals[i] != last) {
            _a025055_solutions[_a025055_solutions_count++] = all_vals[i];
            last = all_vals[i];
            first = 0;
        }
    }

    free(all_vals); /* free temporary list */

    /* Count odd positions. */
    _a025055_odd_positions = (idx_t *)malloc((size_t)_a025055_solutions_count * sizeof(idx_t));
    if (!_a025055_odd_positions) {
        free(_a025055_solutions);
        return;
    }

    _a025055_odd_count = 0;
    for (idx_t i = 0; i < _a025055_solutions_count; ++i) {
        /* Use modulus to avoid missing operators in big integer types. */
        if ((_a025055_solutions[i] % (seq_t)2) != (seq_t)0) {
            _a025055_odd_positions[_a025055_odd_count++] = i + 1; /* 1-based index */
        }
    }

    _a025055_initialized = 1;
}

/* Public function as required. */
seq_t a(idx_t n)
{
    if (!_a025055_initialized)
        _a025055_init();

    if (n == 0 || _a025055_odd_count == 0 || n > _a025055_odd_count) {
        /* Out of range; return 0 as a safe fallback. */
        return (seq_t)0;
    }

    return (seq_t)_a025055_odd_positions[n - 1];
}
