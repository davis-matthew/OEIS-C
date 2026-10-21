/*
 * A204085  Consider integers m such that both m and 2m are decimal pandigital
 *          numbers (A050278). The sequence gives indices of m in A050278.
 * https://oeis.org/A204085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   pandigital10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate all 10‑digit pandigital numbers, record the
 * indices (in A050278) of those whose double is also a pandigital number. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pandigital10.h"

static bool next_perm(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) j--;
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        l++; r--;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of qualifying indices */
    static seq_t indices[200000];
    static size_t len   = 0;
    static bool initialized = false;

    if (!initialized) {
        int digits[10] = {0,1,2,3,4,5,6,7,8,9};
        size_t pand_idx = 0;            /* index in A050278 */

        do {
            if (digits[0] != 0) {        /* skip numbers with leading 0 */
                pand_idx++;              /* valid pandigital so far */

                unsigned long long m = 0;
                for (int i = 0; i < 10; ++i)
                    m = m * 10 + (unsigned long long)digits[i];

                unsigned long long m2 = m * 2ULL;
                if (pandigital10(m2)) {
                    indices[len++] = pand_idx;
                }
            }
        } while (next_perm(digits, 10));

        initialized = true;
    }

    if (n == 0 || (idx_t)n > (idx_t)len) return 0;
    return indices[(size_t)n - 1];
}
