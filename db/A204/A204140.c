/*
 * A204140  Numbers m such that both m and 7m are zeroless pandigital numbers
 *          (A050289).
 * https://oeis.org/A204140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = number m such that m and 7m are zeroless 1‑9 pandigital
 *            numbers. The sequence contains exactly 24 terms
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Test whether x is a 9‑digit number containing each of 1‑9 exactly once. */
static bool is_pandigital(seq_t x)
{
    bool seen[10] = { false };
    for (int i = 0; i < 9; ++i) {
        unsigned d = (unsigned)(x % 10);
        if (d == 0 || seen[d])
            return false;
        seen[d] = true;
        x /= 10;
    }
    /* x should be zero now (exactly 9 digits). */
    if (x != 0)
        return false;
    for (int d = 1; d <= 9; ++d)
        if (!seen[d])
            return false;
    return true;
}

/* Generate the next lexicographic permutation of array a[0..n-1].
   Returns true if there was a next permutation, false if this was the last. */
static bool next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] > a[i + 1])
        --i;
    if (i < 0)
        return false;
    int j = n - 1;
    while (a[j] < a[i])
        --j;
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        ++l;
        --r;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t seq[24];
    static bool compiled = false;
    if (!compiled) {
        int digits[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        seq_t solutions[24];
        size_t count = 0;
        do {
            seq_t m = 0;
            for (int k = 0; k < 9; ++k)
                m = m * 10 + digits[k];
            seq_t prod = 7 * m;
            if (prod >= 1000000000)          /* not a 9‑digit number */
                continue;
            if (!is_pandigital(prod))
                continue;
            solutions[count++] = m;
        } while (next_permutation(digits, 9));
        /* The sequence of permutations is already in ascending order,
           so we can copy directly. */
        for (size_t i = 0; i < count; ++i)
            seq[i] = solutions[i];
        compiled = true;
    }
    if (n == 0)
        return 0;
    if (n <= 24)
        return seq[n - 1];
    /* For n > 24 the OEIS sequence is not defined; return 0. */
    return 0;
}
