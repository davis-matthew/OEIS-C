/*
 * A189323  Number of nondecreasing arrangements of n+2 numbers in 0..6 with
 *          the last equal to 6 and each after the second equal to the sum of
 *          one or two of the preceding four.
 * https://oeis.org/A189323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 101, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189323
   a(n) = number of nondecreasing arrangements of n+2 numbers in 0..6
   with the last element equal to 6 and each element after the second
   equal to the sum of one or two of the preceding four.
   For n > 8: a(n) = 6*n^2 + 34*n - 264.   */

/* include required headers */
#include <stddef.h>
#include <stdbool.h>

/* Small‑n brute force (n <= 8).  n+2 elements, the last is 6. */
static size_t _small_L;          /* sequence length n+2 */
static int    _small_arr[10];    /* holds the first n+1 elements (last fixed 6) */
static unsigned long long _small_count;

/* checks whether value val at position i can be generated from
   the preceding four elements in _small_arr.  Distinct indices
   are required for the sum of two. */
static bool _check_constraint(size_t i, int val)
{
    size_t start = (i >= 4) ? i - 4 : 0;
    for (size_t j = start; j < i; ++j) {
        if (_small_arr[j] == val) return true;
        for (size_t k = j + 1; k < i; ++k) {
            if (_small_arr[j] + _small_arr[k] == val) return true;
        }
    }
    return false;
}

/* recursively build the first _small_L-1 elements and count
   sequences that are valid and end with 6. */
static void _dfs_small(size_t pos)
{
    if (pos == _small_L - 1) {                /* all previous positions fixed */
        if (_check_constraint(_small_L - 1, 6))
            ++_small_count;
        return;
    }
    int start = (pos == 0) ? 0 : _small_arr[pos - 1];
    for (int v = start; v <= 6; ++v) {
        _small_arr[pos] = v;
        if (pos >= 2 && !_check_constraint(pos, v)) continue;
        _dfs_small(pos + 1);
    }
}

/* wrapper for small n (n <= 8). */
static seq_t _count_small(idx_t n)
{
    _small_L = (size_t)n + 2;
    _small_count = 0;
    _dfs_small(0);
    return (seq_t)_small_count;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 8) return _count_small(n);
    seq_t nn = (seq_t)n;
    return 6 * nn * nn + 34 * nn - 264;
}
