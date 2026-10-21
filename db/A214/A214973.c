/*
 * A214973  Number of terms in greedy representation of n using Fibonacci and
 *          Lucas numbers.
 * https://oeis.org/A214973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms in the greedy representation of n using
 *            Fibonacci and Lucas numbers. This implementation builds the set
 *            of Fibonacci and Lucas numbers <= n, removes duplicates, sorts
 *            the set, and then performs the greedy subtraction
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The following function is defined with external linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n<1 */

    /* --- Generate Fibonacci numbers <= n -------------------------------- */
    seq_t fib[128];
    size_t fib_cnt = 0;

    seq_t f_prev = 1, f_curr = 1;
    if (f_prev <= (seq_t)n) fib[fib_cnt++] = f_prev;
    if (f_curr <= (seq_t)n) fib[fib_cnt++] = f_curr;

    while (1) {
        seq_t next = f_prev + f_curr;
        if (next > (seq_t)n) break;
        fib[fib_cnt++] = next;
        f_prev = f_curr;
        f_curr = next;
    }

    /* --- Generate Lucas numbers <= n ------------------------------------- */
    seq_t luc[128];
    size_t luc_cnt = 0;

    seq_t l_prev = 2, l_curr = 1;
    if (l_prev <= (seq_t)n) luc[luc_cnt++] = l_prev;
    if (l_curr <= (seq_t)n) luc[luc_cnt++] = l_curr;

    while (1) {
        seq_t next = l_prev + l_curr;
        if (next > (seq_t)n) break;
        luc[luc_cnt++] = next;
        l_prev = l_curr;
        l_curr = next;
    }

    /* --- Merge the two arrays (duplicates are allowed for now) ----------- */
    seq_t combo[256];
    size_t combo_cnt = 0;

    for (size_t i = 0; i < fib_cnt; ++i)   combo[combo_cnt++] = fib[i];
    for (size_t i = 0; i < luc_cnt; ++i)   combo[combo_cnt++] = luc[i];

    /* --- Sort the combined array using insertion sort ------------------- */
    for (size_t i = 1; i < combo_cnt; ++i) {
        seq_t key = combo[i];
        size_t j = i;
        while (j > 0 && combo[j - 1] > key) {
            combo[j] = combo[j - 1];
            --j;
        }
        combo[j] = key;
    }

    /* --- Remove duplicates to obtain the unique sorted set ---------------- */
    seq_t uniq[256];
    size_t uniq_cnt = 0;

    if (combo_cnt > 0) {
        uniq[uniq_cnt++] = combo[0];
        for (size_t i = 1; i < combo_cnt; ++i) {
            if (combo[i] != combo[i - 1])
                uniq[uniq_cnt++] = combo[i];
        }
    }

    /* --- Greedy subtraction to count the terms --------------------------- */
    seq_t remaining = (seq_t)n;
    seq_t count = 0;
    size_t idx = uniq_cnt - 1;            /* index of the largest element */

    while (remaining > 0) {
        /* Find the largest element <= remaining.  Since the array is
           sorted ascending, we can move left if necessary.           */
        while (remaining < uniq[idx]) {
            if (idx == 0) break;          /* safety, should never happen */
            --idx;
        }
        remaining -= uniq[idx];
        ++count;
    }

    return count;
}
