/*
 * A023795  Katadromes: digits in base 14 are in strict descending order.
 * https://oeis.org/A023795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 6622, u64 n <= 9111,
 *            u128 n <= 9111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth smallest base‑14 number whose digits are in strictly descending order.
 * Each term corresponds to a non‑empty subset of {0,…,13} arranged in descending order,
 * interpreted as a base‑14 integer.  There are 2^14‑1 = 16383 terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_DIGITS 14
#define NUM_SUBSETS ((1ULL << NUM_DIGITS) - 1)   /* 16383 */

static seq_t  values[NUM_SUBSETS];
static bool   initialized = false;

/* Compare two seq_t values for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Build all numbers that have strictly descending base‑14 digits, then sort them. */
static void init_values(void)
{
    /* For every non‑empty subset of {0,…,13} */
    for (uint64_t mask = 1; mask <= NUM_SUBSETS; ++mask)
    {
        seq_t v = 0;
        /* Scan digits from high to low; each selected digit becomes the next less
           significant digit of the base‑14 number.               */
        for (int i = NUM_DIGITS - 1; i >= 0; --i)
        {
            if (mask & ((uint64_t)1 << i))
                v = v * 14 + (seq_t)i;
        }
        values[mask - 1] = v;      /* store 0‑based index */
    }
    /* Sort the array so that a(1) is the smallest, a(2) the next, etc. */
    qsort(values, NUM_SUBSETS, sizeof(seq_t), cmp_seq);
    initialized = true;
}

/* External linkage: return the nth term of the sequence (a(1) = 0). */
seq_t a(idx_t n)
{
    if (!initialized)
        init_values();

    if (n < 1 || n > NUM_SUBSETS)
        return (seq_t)0;          /* undefined for out‑of‑range indices */

    return values[(size_t)(n - 1)];
}
