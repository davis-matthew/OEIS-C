/*
 * A203603  Numbers n=a+b+c+d such that 0<a<b<c<d and concatenation(n,a,b,c,d)
 *          is a permutation of the digits 0,...,9.
 * https://oeis.org/A203603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A203603 by enumerating all 10! permutations of the digits 0‑9.
   For each permutation the pattern (3,1,1,2,3) or (3,1,2,2,2) is interpreted
   as n,a,b,c,d.  If 0<a<b<c<d, n = a+b+c+d and the concatenation
   n,a,b,c,d uses every digit 0‑9 exactly once, the value of n is kept.
   The values are sorted and deduped; subsequent calls reuse the cached
   list.  The function works for any seq_t and idx_t supplied by the
   build system.           */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Forward declaration of comparison function for qsort. */
static int seq_compare(const void *p1, const void *p2);

/* Returns the next lexicographic permutation of array a with length n.
 * Returns true if such a permutation exists, false if the array is
 * already in its last permutation. */
static bool next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1])
        i--;
    if (i < 0)
        return false;

    int j = n - 1;
    while (a[j] <= a[i])
        j--;

    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;

    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        l++;
        r--;
    }
    return true;
}

/* Comparison function for qsort, used to sort seq_t values. */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b)
        return -1;
    else if (*a > *b)
        return 1;
    else
        return 0;
}

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* Static cache of terms. */
    static bool initialized = false;
    static seq_t *terms = NULL;
    static size_t term_count = 0;

    if (!initialized) {
        /* Allocate an initial buffer for the terms. */
        size_t alloc_cap = 1024;
        terms = (seq_t *)malloc(alloc_cap * sizeof(seq_t));
        size_t len = 0;

        /* Initial permutation [0,1,2,3,4,5,6,7,8,9]. */
        int digits[10];
        for (int i = 0; i < 10; ++i)
            digits[i] = i;

        /* Enumerate all 10! permutations. */
        do {
            /* Pattern 1: n(3), a(1), b(1), c(2), d(3) */
            if (digits[0] != 0 && digits[3] != 0 && digits[4] != 0 &&
                digits[5] != 0 && digits[7] != 0) {
                seq_t nval = digits[0] * 100 + digits[1] * 10 + digits[2];
                seq_t a1   = digits[3];
                seq_t b1   = digits[4];
                seq_t c1   = digits[5] * 10 + digits[6];
                seq_t d1   = digits[7] * 100 + digits[8] * 10 + digits[9];
                if (nval == a1 + b1 + c1 + d1 && a1 < b1 && b1 < c1 && c1 < d1) {
                    terms[len++] = nval;
                    if (len == alloc_cap) {
                        alloc_cap <<= 1;
                        terms = (seq_t *)realloc(terms, alloc_cap * sizeof(seq_t));
                    }
                }
            }

            /* Pattern 2: n(3), a(1), b(2), c(2), d(2) */
            if (digits[0] != 0 && digits[3] != 0 && digits[4] != 0 &&
                digits[6] != 0 && digits[8] != 0) {
                seq_t nval = digits[0] * 100 + digits[1] * 10 + digits[2];
                seq_t a1   = digits[3];
                seq_t b1   = digits[4] * 10 + digits[5];
                seq_t c1   = digits[6] * 10 + digits[7];
                seq_t d1   = digits[8] * 10 + digits[9];
                if (nval == a1 + b1 + c1 + d1 && a1 < b1 && b1 < c1 && c1 < d1) {
                    terms[len++] = nval;
                    if (len == alloc_cap) {
                        alloc_cap <<= 1;
                        terms = (seq_t *)realloc(terms, alloc_cap * sizeof(seq_t));
                    }
                }
            }
        } while (next_permutation(digits, 10));

        /* Sort the collected values. */
        qsort(terms, len, sizeof(seq_t), seq_compare);

        /* Deduplicate in-place. */
        size_t uniq = 0;
        for (size_t i = 0; i < len; ++i) {
            if (i == 0 || terms[i] != terms[i - 1]) {
                terms[uniq++] = terms[i];
            }
        }
        term_count = uniq;
        initialized = true;
    }

    if (n == 0 || n > term_count)
        return 0;
    return terms[n - 1];
}
