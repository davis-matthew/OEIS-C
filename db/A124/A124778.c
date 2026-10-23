/*
 * A124778  Number of unlabeled unordered rooted forests associated with
 *          compositions in standard order.
 * https://oeis.org/A124778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A124778: a(n) = product_{i=0}^{k-2} p(c_{i+1},c_i)
   where c is the n‑th composition in standard order (A066099),
   and p(m,k) is the number of partitions of m into ≤k parts. */

#include <stdbool.h>
#include <stddef.h>

static inline idx_t floor_log2(idx_t v)
{
    idx_t r = 0;
    while (v >> 1) {
        v >>= 1;
        ++r;
    }
    return r;
}

/* Pre‑computed partition table:
   part[n][k] = number of partitions of n into at most k parts (0 ≤ n,k ≤ 20). */
static bool _part_init = false;
static seq_t _part[21][21];

static void _init_partitions(void)
{
    if (_part_init) return;
    for (idx_t k = 0; k <= 20; ++k)
        _part[0][k] = (seq_t)1;        /* p(0,k) = 1 */
    for (idx_t n = 1; n <= 20; ++n) {
        _part[n][0] = (seq_t)0;        /* p(n,0) = 0 for n>0 */
        for (idx_t k = 1; k <= 20; ++k) {
            seq_t v = _part[n][k - 1];
            if (n >= k)
                v += _part[n - k][k];
            _part[n][k] = v;
        }
    }
    _part_init = true;
}

seq_t a(idx_t n)
{
    _init_partitions();

    if (n == 0)              /* empty composition */
        return (seq_t)1;

    /* Determine the sum s of the composition:
       compositions of sum s start at index 2^{s-1}. */
    idx_t s = floor_log2(n) + 1;          /* 1 ≤ s ≤ 14 for n ≤ 10000 */
    idx_t start = ((idx_t)1) << (s - 1);
    idx_t r = n - start;                  /* rank within sum s (0‑based) */

    /* Decode the composition from the binary representation of r. */
    uint8_t parts[21];                     /* maximal length 20 */
    size_t len = 0;
    size_t cur = 1;                        /* current part length */

    for (idx_t i = 1; i < s; ++i) {        /* positions 1..s-1 */
        idx_t bit = ((idx_t)1) << (s - 1 - i);
        if (r & bit) {                     /* cut after element i */
            parts[len++] = (uint8_t)cur;
            cur = 1;
        } else {
            ++cur;                         /* extend current part */
        }
    }
    parts[len++] = (uint8_t)cur;            /* final part */

    /* Compute the product of partition numbers for adjacent parts. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i + 1 < len; ++i) {
        result *= _part[(idx_t)parts[i + 1]][(idx_t)parts[i]];
    }
    return result;
}
