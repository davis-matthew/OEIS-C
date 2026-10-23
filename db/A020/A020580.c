/*
 * A020580  Smallest nonempty set S containing prime divisors of 2k+9 for each
 *          k in S.
 * https://oeis.org/A020580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A020580:  Smallest nonempty set S closed under prime divisors
 * of 2k+9 for each k in S; returns the nth smallest element (1‑based). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper to check whether a value is already in the array. */
static inline bool contains(const seq_t *arr, size_t sz, seq_t val)
{
    for (size_t i = 0; i < sz; ++i)
        if (arr[i] == val)
            return true;
    return false;
}

/* Comparator for qsort: works for any integer seq_t type. */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y);
}

/* Add a distinct prime to the dynamic array, growing capacity if needed. */
static inline void add_prime(seq_t **arr, size_t *sz, size_t *cap, seq_t val)
{
    if (contains(*arr, *sz, val))
        return;
    if (*sz == *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*sz] = val;
    ++(*sz);
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    static seq_t *S = NULL;
    static size_t sz = 0, cap = 0;
    static bool initialized = false;

    /* Build the closed set only once. */
    if (!initialized) {
        /* Start with the smallest prime satisfying the condition: 5. */
        add_prime(&S, &sz, &cap, (seq_t)5);

        /* Iterate over elements, expanding the set with new prime divisors. */
        size_t pos = 0;
        while (pos < sz) {
            seq_t k = S[pos++];
            /* Compute 2k+9 safely in 64‑bit integer. */
            uint64_t v = (uint64_t)2 * (uint64_t)k + 9ULL;

            /* Trial division to factor v. */
            uint64_t d = 2;
            while (d * d <= v) {
                if (v % d == 0) {
                    /* d is a prime factor. */
                    add_prime(&S, &sz, &cap, (seq_t)d);
                    while (v % d == 0)
                        v /= d;
                }
                ++d;
            }
            if (v > 1) /* remaining prime factor. */
                add_prime(&S, &sz, &cap, (seq_t)v);
        }

        /* Sort the resulting set for ordered access. */
        qsort(S, sz, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    /* Return 0 if n is out of bounds. */
    if (n <= 0 || (size_t)(n - 1) >= sz)
        return 0;
    return S[(size_t)(n - 1)];
}
