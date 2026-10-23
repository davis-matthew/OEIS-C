/*
 * A035300  Expansion of Sum_{n>=0} (q^n / Product_{k=1..n+4} (1 - q^k)).
 * https://oeis.org/A035300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n+4) - round((n+7)^2/12)
 * Range:     u8 n <= 8, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
/* Allocate and free for C++ (class type) */
static seq_t* allocate_partitions(size_t cap) { return new seq_t[cap]; }
static void free_partitions(seq_t *p) { delete[] p; }
#else
/* Allocate and free for C (built‑in type) */
static seq_t* allocate_partitions(size_t cap) { return (seq_t*)malloc(cap * sizeof(seq_t)); }
static void free_partitions(seq_t *p) { free(p); }
#endif

/* Cache for partition numbers p[0..max_n] */
static seq_t *p_cache = NULL;
static size_t p_cap = 0;

/* Ensure partitions up to `up_to` are computed */
static void ensure_partitions(idx_t up_to)
{
    if ((idx_t)up_to <= (idx_t)p_cap)
        return;

    size_t new_cap = (size_t)up_to + 1;
    seq_t *new_arr = allocate_partitions(new_cap);
    if (!new_arr) return; /* allocation failed – caller will handle zero */

    /* Initialize the new array to zero and copy existing values */
    for (size_t i = 0; i < new_cap; ++i)
        new_arr[i] = 0;
    if (p_cache) {
        for (size_t i = 0; i <= p_cap; ++i)
            new_arr[i] = p_cache[i];
    } else {
        new_arr[0] = 1; /* p(0) = 1 */
    }

    /* Compute new partition numbers */
    for (idx_t n = (idx_t)p_cap + 1; n <= up_to; ++n) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (long long k = 1; ; ++k) {
            long long g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            if (k & 1) pos += new_arr[n - g1];
            else      neg += new_arr[n - g1];
            long long g2 = k * (3 * k + 1) / 2;
            if (g2 <= n) {
                if (k & 1) pos += new_arr[n - g2];
                else      neg += new_arr[n - g2];
            }
        }
        new_arr[n] = pos - neg; /* pos >= neg for the partition function */
    }

    /* Replace old cache */
    if (p_cache)
        free_partitions(p_cache);
    p_cache = new_arr;
    p_cap   = up_to;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n + 4 < n) /* overflow check, unlikely for given limits */
        return 0;
    idx_t up_to = n + 4;
    ensure_partitions(up_to);

    seq_t part = p_cache[up_to];               /* partition p(n+4) */
    seq_t base = (seq_t)(n + 7);
    seq_t sq   = base * base;
    seq_t round_term = (sq + 6) / 12;          /* round((n+7)^2/12) */
    return part - round_term;
}
