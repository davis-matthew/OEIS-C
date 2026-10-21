/*
 * A051018  Numbers that are 2-persistent but not 3-persistent.
 * https://oeis.org/A051018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search for 10‑digit pandigital m with m and 2m pandigital, 3m
 *            not
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_pandigital10_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_pandigital10_local.h"

/* Classic next_permutation: returns true if a higher permutation exists. */
static bool next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int l = i + 1, r = n - 1; l < r; ++l, --r)
    {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return true;
}

/* Sequence generator for A051018. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *solutions = NULL;
    static size_t solutions_count = 0;
    static size_t solutions_cap   = 0;

    if (!initialized)
    {
        /* Initial capacity; reallocate if needed. */
        solutions_cap = 2000;
        solutions = (seq_t *)malloc(solutions_cap * sizeof(seq_t));
        solutions_count = 0;

        int digits[10];
        for (int i = 0; i < 10; ++i) digits[i] = i;

        bool has_next = true;
        do
        {
            if (digits[0] == 0)   /* skip leading zero */
            {
                has_next = next_permutation(digits, 10);
                continue;
            }

            uint64_t m = 0;
            for (int i = 0; i < 10; ++i)
                m = m * 10ULL + (uint64_t)digits[i];

            uint64_t m2 = m * 2ULL;
            uint64_t m3 = m * 3ULL;

            if (is_pandigital10_local(m2) && !is_pandigital10_local(m3))
            {
                if (solutions_count == solutions_cap)
                {
                    solutions_cap <<= 1;
                    solutions = (seq_t *)realloc(solutions,
                                                solutions_cap * sizeof(seq_t));
                }
                solutions[solutions_count++] = (seq_t)m;
            }
            has_next = next_permutation(digits, 10);
        } while (has_next);

        initialized = true;
    }

    if (n == 0 || (size_t)(n - 1) >= solutions_count)
        return (seq_t)0;   /* undefined / out of range */

    return solutions[(size_t)(n - 1)];
}
