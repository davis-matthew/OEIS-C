/*
 * A091530  a(n) = Sum_{k=1..n} H(k) k! (n-k)!, where H(k) is the k-th
 *          harmonic number.
 * https://oeis.org/A091530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} H(k) * k! * (n-k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses O(n^2) time with memoisation of factorials,
   the auxiliary sequence A(k)=H(k)*k!, and the result sequence a(n).
   It works for both C (integer) builds and C++ (arbitrary‑precision)
   builds. */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

static idx_t max_n = 0;        /* largest n for which we have fact, A and a */

#ifdef __cplusplus
static std::vector<seq_t> fact_vec;
static std::vector<seq_t> A_vec;
static std::vector<seq_t> a_vec;
static idx_t max_a_done = 0;   /* largest n for which a(n) is computed */
#else
static seq_t *fact_arr = NULL;
static seq_t *A_arr    = NULL;
static seq_t *a_arr    = NULL;
static idx_t max_a_done = 0;
#endif

/* Helper to ensure factorials and auxiliary sequence A are known up to n. */
static void ensure_fact_A(idx_t n)
{
    if (n <= max_n) return;

#ifdef __cplusplus
    fact_vec.resize(n + 1);
    A_vec.resize(n + 1);
    if (max_n == 0) {
        fact_vec[0] = (seq_t)1;
        A_vec[0]    = (seq_t)0;          /* A(0)=0 for recurrence convenience */
    }
    for (idx_t k = max_n + 1; k <= n; ++k) {
        fact_vec[k] = fact_vec[k - 1] * (seq_t)k;
        A_vec[k]    = (seq_t)k * A_vec[k - 1] + fact_vec[k - 1];
    }
#else
    /* realloc handles NULL pointer correctly */
    fact_arr = (seq_t *)realloc(fact_arr, (n + 1) * sizeof(seq_t));
    A_arr    = (seq_t *)realloc(A_arr,    (n + 1) * sizeof(seq_t));
    a_arr    = (seq_t *)realloc(a_arr,    (n + 1) * sizeof(seq_t));
    if (max_n == 0) {
        fact_arr[0] = (seq_t)1;
        A_arr[0]    = (seq_t)0;
    }
    for (idx_t k = max_n + 1; k <= n; ++k) {
        fact_arr[k] = fact_arr[k - 1] * (seq_t)k;
        A_arr[k]    = (seq_t)k * A_arr[k - 1] + fact_arr[k - 1];
    }
#endif
    max_n = n;
}

/* Helper to ensure a(n) values are computed up to n. */
static void ensure_a_values(idx_t n)
{
    if (n <= max_a_done) return;

#ifdef __cplusplus
    a_vec.resize(n + 1);
    for (idx_t m = max_a_done + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += A_vec[k] * fact_vec[m - k];
        }
        a_vec[m] = sum;
    }
#else
    for (idx_t m = max_a_done + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += A_arr[k] * fact_arr[m - k];
        }
        a_arr[m] = sum;
    }
#endif
    max_a_done = n;
}

/* Main entry */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, but return 0 */

    ensure_fact_A(n);          /* factorials and A up to n */
    ensure_a_values(n);        /* compute a(k) up to n if needed */

#ifdef __cplusplus
    return a_vec[n];
#else
    return a_arr[n];
#endif
}
