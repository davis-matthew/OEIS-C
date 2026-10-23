/*
 * A008827  Number of proper partitions of a set of n labeled elements.
 * https://oeis.org/A008827
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = B(n) - 2, where B(n) is the Bell number.
 * B(n) is computed by the standard recurrence:
 *   B(0) = 1
 *   B(n) = Σ_{k=0}^{n-1} C(n-1, k) * B(k)   (n ≥ 1)
 *
 * The implementation caches all previously computed Bell numbers, so
 * repeated calls to a(idx_t n) are answered in amortised O(1) after the
 * first O(n²) computation.  It works for any seq_t type allowed by the
 * build system, including arbitrary‑precision integers.
 */

#include <stdlib.h>

/* Static storage for computed Bell numbers. */
static seq_t *bell = NULL;          /* array of Bell numbers B(0)..B(max)   */
static idx_t bell_cap = 0;          /* current size of 'bell'                */
static idx_t bell_last = 0;         /* highest n for which B(n) is known     */

/* Ensure that the array 'bell' can hold indices up to 'need-1'.  The
 * array is grown by reallocation; new elements are zero‑initialised.
 */
static void bell_resize(idx_t need)
{
    if (need <= bell_cap)
        return;

    seq_t *new_arr = (seq_t *)malloc(need * sizeof(seq_t));
    if (!new_arr)          /* out of memory – abort silently */
        return;

    for (idx_t i = 0; i < bell_cap; ++i)
        new_arr[i] = bell[i];
    for (idx_t i = bell_cap; i < need; ++i)
        new_arr[i] = (seq_t)0;

    free(bell);
    bell = new_arr;
    bell_cap = need;
}

/* Precompute Bell numbers up to index n (inclusive). */
static void bell_compute_upto(idx_t n)
{
    if (bell_cap == 0) {          /* first time: create B(0)=1 */
        bell_resize(n + 1);
        bell[0] = (seq_t)1;
        bell_last = 0;
    }

    if (n <= bell_last)           /* already computed */
        return;

    if (bell_cap <= n)
        bell_resize(n + 1);

    /* Compute from (bell_last + 1) up to n */
    for (idx_t i = bell_last + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;          /* bin = C(i-1, 0) */

        for (idx_t k = 0; k < i; ++k) {
            if (k > 0) {
                bin = bin * (seq_t)(i - k);
                bin = bin / (seq_t)k;
            }
            sum += bin * bell[k];
        }
        bell[i] = sum;
    }
    bell_last = n;
}

/* External function required by the judge. */
seq_t a(idx_t n)
{
    if (n < 2)                      /* the offset of the sequence is 2 */
        return (seq_t)0;

    bell_compute_upto(n);

    seq_t res = bell[n];
    /* subtract 2, safe because B(n) ≥ 2 for n≥2 */
    if (res > (seq_t)1)
        res = res - (seq_t)2;
    else
        res = (seq_t)0;

    return res;
}
