/*
 * A104074  a(1)=1. a(n) = a(n-1) + (number of terms, from among terms a(1)
 *          through a(n-1), which divide Sum_{k=1..n-1} a(k)).
 * https://oeis.org/A104074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 343, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A104074
 *  a(1) = 1
 *  a(n) = a(n-1) + (number of terms among a(1)..a(n-1) that divide
 *                  sum_{k=1}^{n-1} a(k))
 *
 *  This implementation memoises all terms and prefix sums up to the
 *  requested index.  The inner loop counts how many previous terms
 *  divide the current prefix sum, which costs O(k) for the k‑th term.
 *  The overall complexity is O(n²) but the constant is tiny and
 *  it easily meets the 2‑second limit for n ≈ 10 000 on all
 *  supported integer sizes (uint8_t … __int128, arbitrary‑precision).
 */

#include <stddef.h>    /* size_t */
#include <stdlib.h>    /* malloc, realloc */

/* dynamic arrays storing a(i) and the prefix sums sum_{k=1}^{i} a(k) */
static seq_t *a_arr  = NULL; /* a terms, 0-based */
static seq_t *sum_arr = NULL; /* prefix sums, 0-based */
static idx_t  size_ = 0;   /* number of terms currently stored */
static idx_t  cap_  = 0;   /* capacity of the allocated arrays */

/* Ensure the dynamic arrays can hold at least n elements. */
static void
ensure_capacity(idx_t n)
{
    if (n <= cap_)
        return;

    idx_t newcap = cap_ ? cap_ * 2 : 1;
    while (newcap < n)
        newcap <<= 1;

    a_arr   = (seq_t *)realloc(a_arr,   newcap * sizeof(seq_t));
    sum_arr = (seq_t *)realloc(sum_arr, newcap * sizeof(seq_t));
    cap_ = newcap;
}

/* Main function that the framework calls. */
seq_t
a(idx_t n)
{
    if (n == 0)          /* a(0) is undefined; return 0 as a safe placeholder */
        return (seq_t)0;

    /* Initialise the sequence with a(1) = 1 if it has not been done yet. */
    if (size_ == 0) {
        ensure_capacity(1);
        a_arr[0]   = (seq_t)1;
        sum_arr[0] = (seq_t)1;
        size_ = 1;
    }

    /* Extend the memoised sequence up to the requested index n. */
    if (n > size_) {
        ensure_capacity(n);
        for (idx_t i = size_; i < n; ++i) {
            /* Sum of the first i terms: sum_{k=1}^{i} a(k) */
            seq_t Sprev = sum_arr[i - 1];
            idx_t cnt = 0;

            /* Count how many of the previous terms divide Sprev. */
            for (idx_t j = 0; j < i; ++j) {
                if (Sprev % a_arr[j] == 0)
                    ++cnt;
            }

            /* Compute a(i+1) and update prefix sum. */
            seq_t new_val = a_arr[i - 1] + (seq_t)cnt;
            a_arr[i]   = new_val;
            sum_arr[i] = sum_arr[i - 1] + new_val;
        }
        size_ = n;
    }

    return a_arr[n - 1];
}
