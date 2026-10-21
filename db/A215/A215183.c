/*
 * A215183  Number of 3 X 3 X 3 triangular 0..n arrays with every horizontal
 *          row nondecreasing, first elements of rows nonincreasing, last
 *          elements of rows nondecreasing, and every row having the same
 *          average value.
 * https://oeis.org/A215183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A215183:
   a(n) = 2*a(n-1) - 3*a(n-4) + 3*a(n-6) - 2*a(n-9) + a(n-10)  for n ≥ 11
   Initial values:
   a(1)=2, a(2)=5, a(3)=12, a(4)=22, a(5)=38, a(6)=64, a(7)=98,
   a(8)=145, a(9)=210, a(10)=291
   The function computes a(n) for any n≥1, memoising results so that calls
   in arbitrary order are efficient. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *memo = NULL;          /* 1‑based index; memo[0] unused (or 0) */
static idx_t memo_capacity = 0;     /* number of allocated slots (≥ memo_max+1) */
static idx_t memo_max = 0;          /* largest n for which a(n) is already computed */

/* Ensure the memo array can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= memo_capacity)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem)
        abort();                    /* Out of memory */

    /* initialise newly allocated slots to zero */
    for (idx_t i = memo_capacity; i <= n; ++i)
        new_mem[i] = (seq_t)0;

    memo = new_mem;
    memo_capacity = new_cap;
}

/* Initialise base values a(1) … a(10).  Called only once. */
static void init_base_values(void)
{
    if (memo_capacity < 11)        /* need slots 1…10 at least */
        ensure_capacity(10);

    memo[1] = (seq_t)2;
    memo[2] = (seq_t)5;
    memo[3] = (seq_t)12;
    memo[4] = (seq_t)22;
    memo[5] = (seq_t)38;
    memo[6] = (seq_t)64;
    memo[7] = (seq_t)98;
    memo[8] = (seq_t)145;
    memo[9] = (seq_t)210;
    memo[10] = (seq_t)291;

    memo_max = 10;
}

/* Main sequence function.  External linkage required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1; safe fallback */

    /* Ensure array capacity and initialise base values if needed. */
    if (memo == NULL)
        init_base_values();

    ensure_capacity(n);

    /* If the value is already known, return it. */
    if (n <= memo_max)
        return memo[n];

    /* Compute missing terms using the recurrence. */
    for (idx_t i = memo_max + 1; i <= n; ++i) {
        /* i ≥ 11 so all referenced indices are valid. */
        seq_t val = 2 * memo[i - 1];
        val -= 3 * memo[i - 4];
        val += 3 * memo[i - 6];
        val -= 2 * memo[i - 9];
        val += memo[i - 10];
        memo[i] = val;
    }

    memo_max = n;
    return memo[n];
}
