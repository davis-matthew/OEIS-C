/*
 * A173280  First column of the matrix power A173279(.,.)^j in the limit
 *          j->infinity.
 * https://oeis.org/A173280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{t=0..n, n-t even} t! * a((n-t)/2) Recurrence:
 *            a(0)=1, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_one.h"
#include "../utils/free_seq.h"

/* memoization storage */
static seq_t *a_arr   = NULL;
static size_t a_cap   = 0;   /* number of elements currently allocated in a_arr */

static seq_t *fact_arr = NULL;
static size_t fact_cap = 0;   /* number of elements currently allocated in fact_arr */

/* ensure factorials 0! … n! are available */
static void ensure_factorials(idx_t n)
{
    if (fact_arr == NULL) {
        fact_arr = alloc_seq_array((size_t)n + 1);
        fact_cap = 0;
    }

    /* initialise 0! if it was never done */
    if (fact_cap == 0) {
        fact_arr[0] = seq_one();
        fact_cap = 1;
    }

    if (fact_cap > (size_t)n)
        return;   /* already have enough factorials */

    /* enlarge the array */
    seq_t *new_arr = alloc_seq_array((size_t)n + 1);
    for (size_t i = 0; i < fact_cap; ++i) {
        new_arr[i] = fact_arr[i];
    }
    if (fact_arr) free_seq(fact_arr);
    fact_arr = new_arr;

    /* compute missing factorials */
    for (idx_t i = (idx_t)fact_cap; i <= n; ++i) {
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
    }
    fact_cap = (size_t)n + 1;
}

/* ensure a(n) is known for all n <= N */
static void compute_upto(idx_t N)
{
    size_t old_cap = a_cap;

    if (a_cap <= N) {
        seq_t *new_arr = alloc_seq_array((size_t)N + 1);
        for (size_t i = 0; i < old_cap; ++i) {
            new_arr[i] = a_arr[i];
        }
        if (a_arr) free_seq(a_arr);
        a_arr = new_arr;
        a_cap = (size_t)N + 1;
    }

    /* base cases */
    if (a_cap > 0) a_arr[0] = seq_one();
    if (N >= 1)    a_arr[1] = seq_one();

    /* factorials up to N are needed */
    ensure_factorials(N);

    /* compute missing values */
    idx_t start = (old_cap > 1) ? (idx_t)old_cap : (idx_t)2;
    if (start > N) return;

    for (idx_t k = start; k <= N; ++k) {
        seq_t sum = 0;
        for (idx_t t = 0; t <= k; ++t) {
            if (((k - t) & 1) == 0) {
                idx_t idx = (k - t) >> 1;
                sum = sum + fact_arr[t] * a_arr[idx];
            }
        }
        a_arr[k] = sum;
    }
}

seq_t a(idx_t n)
{
    compute_upto(n);
    return a_arr[n];
}
