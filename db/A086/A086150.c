/*
 * A086150  Number of permutations of decimal digits of n which yield nonprime
 *          numbers.
 * https://oeis.org/A086150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of permutations of the decimal digits of n
 *            (leading zeros allowed) that yield non‑prime numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_char
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_char.h"

/* Return true iff v is a prime number (0 or 1 is not prime). */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2)
        return false;
    if (v % 2ULL == 0ULL)
        return v == 2ULL;
    if (v % 3ULL == 0ULL)
        return v == 3ULL;
    for (uint64_t d = 5ULL; d * d <= v; d += 6ULL) {
        if (v % d == 0ULL || v % (d + 2ULL) == 0ULL)
            return false;
    }
    return true;
}

/* Standard next_permutation for an array of chars (lexicographic). */
static bool next_permutation(char *a, size_t n)
{
    if (n < 2)
        return false;

    int64_t i = (int64_t)n - 2;
    while (i >= 0 && a[i] >= a[i + 1])
        --i;
    if (i < 0)
        return false;                 /* last permutation reached */

    int64_t j = (int64_t)n - 1;
    while (a[j] <= a[i])
        --j;

    /* swap a[i] and a[j] */
    char tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;

    /* reverse suffix a[i+1 .. n-1] */
    size_t l = (size_t)(i + 1);
    size_t r = n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        ++l;
        --r;
    }
    return true;
}

/* OEIS sequence A086150: number of non‑prime permutations of decimal digits of n. */
seq_t a(idx_t n)
{
    /* Convert n to a decimal string. */
    char str[32];
    int len = snprintf(str, sizeof(str), "%llu", (unsigned long long)n);
    if (len <= 0)
        return 0;               /* should never happen for n >= 1 */

    /* Sort digits to start from the first lexicographic permutation. */
    qsort(str, (size_t)len, sizeof(char), cmp_char);

    seq_t count = 0;
    do {
        /* Build the integer represented by the current permutation. */
        uint64_t val = 0;
        for (int k = 0; k < len; ++k)
            val = val * 10ULL + (uint64_t)(str[k] - '0');

        /* Count if the value is not prime (includes 0 and 1). */
        if (!is_prime_uint64(val))
            ++count;
    } while (next_permutation(str, (size_t)len));

    return count;
}
