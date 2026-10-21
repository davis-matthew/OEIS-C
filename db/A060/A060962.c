/*
 * A060962  Expansion of (1+x^2)*(1+x^5)/( Product_{j=1..7} (1-x^j) ).
 * https://oeis.org/A060962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_{≤7}(n) + p_{≤7}(n‑2) + p_{≤7}(n‑5) + p_{≤7}(n‑7) where
 *            p_{≤7}(m) is the number of partitions of m into parts of size at
 *            most 7. The generating function is (1 + x² + x⁵ + x⁷) /
 *            ∏_{j=1}^7 (1 – x^j)
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 386, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static storage for partition numbers p_{≤7}(i) */
static seq_t *part = NULL;   /* dynamic array of values */
static size_t part_len = 0;  /* current number of stored values (part_len-1 is max index) */

/* recompute partitions up to N inclusive (N is size_t) */
static void compute_partitions(size_t N)
{
    if (N + 1 <= part_len)          /* already have enough values */
        return;

    /* allocate/expand array */
    seq_t *tmp = (seq_t *)realloc(part, (N + 1) * sizeof(seq_t));
    if (!tmp) return;               /* allocation failed – abort silently */
    part = tmp;
    part_len = N + 1;

    /* recompute from scratch */
    for (size_t i = 0; i < part_len; ++i)
        part[i] = (seq_t)0;
    part[0] = (seq_t)1;             /* one partition of 0 */

    for (size_t part_size = 1; part_size <= 7; ++part_size) {
        for (size_t s = part_size; s < part_len; ++s)
            part[s] += part[s - part_size];
    }
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    compute_partitions(N);

    seq_t res = part[N];
    if (n >= 2) res += part[N - 2];
    if (n >= 5) res += part[N - 5];
    if (n >= 7) res += part[N - 7];
    return res;
}
