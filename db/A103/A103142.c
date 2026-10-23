/*
 * A103142  a(n) = 2*a(n-1) + a(n-2) + a(n-3) + a(n-4), with a(0)=1, a(1)=2,
 *          a(3)=5, a(4)=13.
 * https://oeis.org/A103142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3) + a(n-4) with a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=13. Cached iterative evaluation with dynamic array
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values: memo[i] holds a(i) for 0 <= i <= sz. */
static seq_t *memo = NULL;    /* pointer to the cached array */
static idx_t  capacity = 0;   /* current allocation size (>= sz+1) */
static idx_t  sz = 0;         /* highest index that has been computed */

/* Ensure that the cache can hold at least "new_cap" elements. */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= capacity && memo != NULL)
        return;                 /* already enough space */

    /* Allocate new array */
    seq_t *new_mem = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    if (new_mem == NULL)
        abort();                /* out of memory, stop program */

    /* Copy existing values if any */
    if (memo != NULL) {
        for (idx_t i = 0; i <= sz; ++i)
            new_mem[i] = memo[i];
        free(memo);
    }

    memo = new_mem;
    capacity = new_cap;
}

/* Return the n-th term of A103142. */
seq_t a(idx_t n)
{
    /* Handle very small indices directly to avoid any array access. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)13;

    /* First time call: create the cache with space for 0..3. */
    if (memo == NULL) {
        ensure_capacity(4);          /* capacity for indices 0..3   */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)2;
        memo[2] = (seq_t)5;
        memo[3] = (seq_t)13;
        sz = 3;
    }

    /* If the wanted index is already computed, just return it. */
    if (n <= sz)
        return memo[n];

    /* Expand cache to fit up to n. */
    ensure_capacity(n + 1);

    /* Compute the missing terms iteratively. */
    for (idx_t i = sz + 1; i <= n; ++i) {
        memo[i] = 2 * memo[i - 1] + memo[i - 2] + memo[i - 3] + memo[i - 4];
    }
    sz = n;

    return memo[n];
}
