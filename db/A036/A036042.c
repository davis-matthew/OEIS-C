/*
 * A036042  k appears partition(k) times.
 * https://oeis.org/A036042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that Σ_{i=0}^{k} p(i) > n, where p(i) is
 *            the number of integer partitions of i
 * Range:     u8 n <= 97, u16 n <= 9295, u32 n <= 9295, u64 n <= 9295,
 *            u128 n <= 9295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static arrays to memoize partition numbers and cumulative sums */
static seq_t *part = NULL;   /* partition numbers p(k)  */
static seq_t *cuml = NULL;   /* cumulative sums S(k) = Σ_{i=0}^{k} p(i) */
static size_t max_k = 0;     /* highest k computed so far  */
static size_t capacity = 0;  /* current capacity of the arrays   */

static void
ensure_capacity(size_t needed)
{
    if (needed <= capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    part = (seq_t *)realloc(part, new_cap * sizeof(seq_t));
    cuml = (seq_t *)realloc(cuml, new_cap * sizeof(seq_t));
    capacity = new_cap;
}

/* compute the partition number p(n) using the generalized pentagonal
   number theorem. Assumes part[0..n-1] are already computed. */
static seq_t
partition_at(size_t n)
{
    seq_t sum = 0;
    for (size_t k = 1;; ++k) {
        size_t pent1 = k * (3 * k - 1) / 2;
        if (pent1 > n)
            break;
        int sign = (k & 1) ? 1 : -1;
        if (sign > 0)
            sum += part[n - pent1];
        else
            sum -= part[n - pent1];

        size_t pent2 = k * (3 * k + 1) / 2;
        if (pent2 > n)
            continue;
        if (sign > 0)
            sum += part[n - pent2];
        else
            sum -= part[n - pent2];
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* initialize arrays on first call */
    if (part == NULL) {
        ensure_capacity(1 + 1);
        part[0]   = 1;   /* p(0) = 1  */
        cuml[0]   = 1;   /* S(0) = 1  */
        max_k = 0;
    }

    /* extend tables until cumulative sum exceeds n */
    while (cuml[max_k] <= n) {
        size_t next = max_k + 1;
        ensure_capacity(next + 1);
        part[next] = partition_at(next);
        cuml[next] = cuml[max_k] + part[next];
        max_k = next;
    }
    return (seq_t)max_k;
}
