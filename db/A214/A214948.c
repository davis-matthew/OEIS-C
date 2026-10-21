/*
 * A214948  a(n) is the sum over all proper integer partitions of n of the
 *          previous terms.
 * https://oeis.org/A214948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Σ_{p∈P*(n)} Σ_{i∈p} a(i) Recurrence: a(1) = 1 a(n) =
 *            Σ_{i=1}^{n-1} a(i) * Σ_{k=1}^{⌊n/i⌋} p(n - k·i) where p(m) is
 *            the number of integer partitions of m. The algorithm precomputes
 *            p(0…N) with the pentagonal recurrence and then evaluates a(n)
 *            iteratively, memoising all lower values
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- memoisation storage ---------- */
static seq_t *a_arr = NULL;       /* a-values, 1‑based, a_arr[0] unused   */
static seq_t *p_arr = NULL;       /* partition numbers p(0…n)             */
static size_t  cap_a = 0;         /* allocated size of a_arr              */
static size_t  cap_p = 0;         /* allocated size of p_arr              */
static idx_t   a_max = 0;         /* largest n computed with a_arr        */
static idx_t   p_max = 0;         /* largest n computed with p_arr        */

/* ---------- utility: grow array ---------- */
static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newCap = (*cap == 0) ? 1 : *cap;
        while (newCap < needed) newCap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newCap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* initialise the newly allocated part */
        for (size_t i = *cap; i < newCap; ++i) tmp[i] = (seq_t)0;
        *arr = tmp;
        *cap = newCap;
    }
}

/* ---------- compute partition numbers p(0…n) ---------- */
static void compute_partitions(idx_t n)
{
    grow_array(&p_arr, &cap_p, (size_t)n + 1);
    if (p_max == 0) {                /* initialise p[0] */
        p_arr[0] = (seq_t)1;
        p_max = 0;
    }
    for (idx_t i = p_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = p_arr[i - pent1];
            if (k & 1) sum += term; else sum -= term;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            term = p_arr[i - pent2];
            if (k & 1) sum += term; else sum -= term;
        }
        p_arr[i] = sum;
    }
    p_max = n;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* unused index */

    if (n > a_max) {
        grow_array(&a_arr, &cap_a, (size_t)n + 1);

        /* ensure partitions up to n are ready */
        if (p_max < n) compute_partitions(n);

        for (idx_t k = a_max + 1; k <= n; ++k) {
            if (k == 1) {
                a_arr[1] = (seq_t)1;
                continue;
            }
            seq_t total = 0;
            for (idx_t i = 1; i < k; ++i) {
                seq_t term_sum = 0;
                for (idx_t t = 1; k >= t * i; ++t) {
                    idx_t rem = k - t * i;
                    term_sum += p_arr[rem];
                }
                total += a_arr[i] * term_sum;
            }
            a_arr[k] = total;
        }
        a_max = n;
    }
    return a_arr[n];
}
