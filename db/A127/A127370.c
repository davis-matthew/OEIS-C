/*
 * A127370  a(1)=1. a(n) = number of positive integers <= n and coprime to
 *          (Sum_{k=1..n-1} a(k)).
 * https://oeis.org/A127370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A127370: a(1)=1; for n>1, 
 * a(n) = #{ m ∈ [1,n]  |  gcd(m, S_{n-1}) = 1 },  
 * where S_k = ∑_{i=1}^k a(i).
 */

#include <stddef.h>
#include <stdlib.h>

/* Euclidean GCD for seq_t. */
static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Dynamic cache for computed terms and their partial sums. */
static idx_t    max_computed = 0;   /* largest n whose a(n) is known */
static seq_t  *a_cache      = NULL; /* a(1)...a(max_computed)   */
static seq_t  *sum_cache    = NULL; /* cumulative sums S_1...S_{max_computed} */
static size_t  capacity     = 0;    /* allocated size of the two arrays */

/* Grow the cache to hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= capacity)
        return;
    size_t new_cap = capacity ? capacity * 2 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    seq_t *new_s = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
    a_cache   = new_a;
    sum_cache = new_s;
    capacity  = new_cap;
}

/* Compute a(n).  The function is thread‑unsafe but satisfies the requirements. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return 0 for safety */
        return (seq_t)0;

    /* Initialise the first term on first call. */
    if (max_computed == 0) {
        ensure_capacity(1);
        a_cache[0]   = (seq_t)1;
        sum_cache[0] = (seq_t)1;
        max_computed = 1;
    }

    /* If we already have a(n), return it. */
    if (n <= max_computed)
        return a_cache[n - 1];

    /* Ensure arrays can hold up to n elements. */
    ensure_capacity((size_t)n);

    /* Running sum S_{max_computed}. */
    seq_t running_sum = sum_cache[max_computed - 1];

    /* Compute terms sequentially up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t count = 0;
        for (idx_t m = 1; m <= i; ++m) {
            if (my_gcd((seq_t)m, running_sum) == (seq_t)1)
                ++count;
        }
        running_sum += count;
        a_cache[i - 1]   = count;
        sum_cache[i - 1] = running_sum;
    }

    max_computed = n;
    return a_cache[n - 1];
}
