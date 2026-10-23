/*
 * A049919  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 3, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n-1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = 3.
 * https://oeis.org/A049919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A049919
 * a(1) = 1, a(2) = 3
 * For n ≥ 3:
 *   let p be the unique integer with 2^p < n-1 ≤ 2^(p+1)
 *   let m = 2*n – 2 – 2^(p+1)
 *   a(n) = a(1)+a(2)+…+a(n-1) – a(m)
 * This implementation memoises values in a dynamic array and keeps a running
 * total of the partial sums for O(1) access to the required sum at each step.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_arr = NULL;     /* a[1] … a[max_n] */
static size_t cap = 0;          /* allocated capacity (≥ max_n+1) */
static idx_t  max_n = 0;        /* largest n that has been computed */
static seq_t  total_sum = 0;    /* sum of a[1] … a[max_n] */

/* floor_log2 for unsigned 64‑bit values */
static idx_t floor_log2_uint64(idx_t v)
{
    idx_t r = 0;
    while (v > 1) { v >>= 1; ++r; }
    return r;
}

/* Ensure capacity for indices up to `need` (inclusive) */
static void ensure_capacity(idx_t need)
{
    if (cap >= (size_t)(need + 1))
        return;

    size_t new_cap = cap ? cap * 2 : 4;
    while (new_cap < (size_t)(need + 1))
        new_cap *= 2;

    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    cap   = new_cap;
}

/* Initialise the first two terms and sum */
static void initialise(void)
{
    if (max_n >= 2)
        return;

    ensure_capacity(2);

    a_arr[1] = 1;      /* a(1) */
    a_arr[2] = 3;      /* a(2) */

    total_sum = a_arr[1] + a_arr[2];
    max_n = 2;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined index, return 0 */

    initialise();

    if (n <= max_n)
        return a_arr[n];

    ensure_capacity(n);

    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        /* Determine p : 2^p < i-1 ≤ 2^(p+1) */
        idx_t p = floor_log2_uint64(i - 2);

        /* Compute m = 2*i – 2 – 2^(p+1) */
        idx_t m = (2 * i - 2) - ((idx_t)1 << (p + 1));

        /* a(i) = total_sum – a(m)  (total_sum = sum of a[1..i-1]) */
        seq_t ai = total_sum - a_arr[m];

        a_arr[i] = ai;
        total_sum += ai;   /* update running sum for next step */
    }

    max_n = n;
    return a_arr[n];
}
