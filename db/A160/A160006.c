/*
 * A160006  a(n) = least number b such that A000404(n) = b^2+c^2, 0 < b <= c.
 * https://oeis.org/A160006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least number b such that A000404(n) = b^2 + c^2, 0 < b <= c.
 * The code precomputes the sequence up to a large enough bound by generating all
 * sums a^2 + b^2 with 1 <= a <= b <= 600, sorting them, deduplicating, and
 * storing the minimal such b for each distinct sum.  It then returns the nth
 * value directly.  A correct qsort comparator is used for ascending order.
 */
#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort on idx_t, ascending order */
static int cmp_idx(const void *p1, const void *p2)
{
    const idx_t v1 = *(const idx_t *)p1;
    const idx_t v2 = *(const idx_t *)p2;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* Integer square root: floor(sqrt(v)) for idx_t */
static idx_t int_sqrt(idx_t v)
{
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == v)
            return mid;
        if (sq < v)
            low = mid + 1;
        else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;
}

/* Minimal b such that x = b^2 + c^2 with b <= c */
static seq_t min_b_idx_t(idx_t x)
{
    idx_t limit = int_sqrt(x / 2);
    for (idx_t b = 1; b <= limit; ++b) {
        idx_t rem = x - b * b;
        idx_t c = int_sqrt(rem);
        if (c * c == rem && c >= b)
            return (seq_t)b;
    }
    return (seq_t)0; /* Should not occur for sums of two squares */
}

/* Parameters for precomputation */
#define LIMIT_B 600U
#define MAX_PAIRS ((LIMIT_B * (LIMIT_B + 1ULL)) / 2)

static idx_t *sums = NULL;          /* temporary storage for a^2 + b^2 */
static seq_t *a_vals = NULL;        /* precomputed a(n) values */
static size_t a_vals_len = 0;
static bool initialized = false;

/* Initialise the lookup table once */
static void init(void)
{
    if (initialized)
        return;

    /* Allocate arrays */
    sums = (idx_t *)malloc(sizeof(idx_t) * MAX_PAIRS);
    a_vals = (seq_t *)malloc(sizeof(seq_t) * MAX_PAIRS);
    size_t sums_cnt = 0;

    /* Generate all a^2 + b^2 for 1 <= a <= b <= LIMIT_B */
    for (idx_t b = 1; b <= LIMIT_B; ++b) {
        for (idx_t a = 1; a <= b; ++a) {
            sums[sums_cnt++] = a * a + b * b;
        }
    }

    /* Sort and remove duplicates while computing the minimal b */
    qsort(sums, sums_cnt, sizeof(idx_t), cmp_idx);

    size_t pos = 0;
    for (size_t i = 0; i < sums_cnt; ++i) {
        if (i == 0 || sums[i] != sums[i - 1]) {
            a_vals[pos++] = min_b_idx_t(sums[i]);
        }
    }
    a_vals_len = pos;

    /* Clean up temporary array */
    free(sums);
    sums = NULL;

    initialized = true;
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1: return 0 */

    init();

    /* Bounds check – should not happen for the test ranges */
    if (n - 1 >= (idx_t)a_vals_len)
        return (seq_t)0;          /* out of precomputed range, return 0 */

    return a_vals[(size_t)(n - 1)];
}
