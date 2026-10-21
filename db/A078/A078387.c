/*
 * A078387  Moebius's mu of numbers which can be written as sum of a positive
 *          square and a positive cube.
 * https://oeis.org/A078387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = μ(A055394(n)) (Moebius's mu of the n‑th number * that can
 *            be written as a positive square plus a positive cube)
 * Range:     i8 n <= 35, i16 n <= 2179, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius.h"

static seq_t *sums = NULL;            /* sorted list of distinct i² + j³ */
static size_t sums_len = 0;           /* number of elements stored */
static idx_t current_limit = 1;       /* current upper bound for sums */

/* Build the array of sums i^2 + j^3 not exceeding 'limit'.
 * The array is sorted in ascending order and contains no duplicates. */
static void build_sums(idx_t limit)
{
    /* Use a byte array as a bitmap of reachable values. */
    uint8_t *seen = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!seen)
        return;

    for (idx_t j = 1; (idx_t)j * j * j <= limit; ++j) {
        idx_t cube = j * j * j;
        /* i ranges while i² + j³ ≤ limit */
        for (idx_t i = 1; (idx_t)i * i <= limit - cube; ++i) {
            seq_t sum = (seq_t)i * i + (seq_t)cube;
            seen[(size_t)sum] = 1;
        }
    }

    /* Count distinct sums. */
    size_t cnt = 0;
    for (idx_t x = 1; x <= limit; ++x)
        if (seen[(size_t)x])
            ++cnt;

    /* Allocate space for the sums. */
    seq_t *new_vals = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!new_vals) {
        free(seen);
        return;
    }

    /* Collect sums in ascending order. */
    size_t pos = 0;
    for (idx_t x = 1; x <= limit; ++x)
        if (seen[(size_t)x])
            new_vals[pos++] = (seq_t)x;

    /* Replace old array. */
    free(sums);
    sums = new_vals;
    sums_len = cnt;

    free(seen);
}

/* Ensure that 'sums' contains at least 'n' distinct numbers. */
static void ensure_sums(idx_t n)
{
    if (sums == NULL)
        build_sums(current_limit);

    while (sums_len < (size_t)n) {
        current_limit *= 2;
        build_sums(current_limit);
        /* Safety guard against runaway growth. */
        if (current_limit > 100000000ULL)
            break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n=0; return 0 */
// Ensure we have enough sums to answer the request.
    ensure_sums(n);

    seq_t num = sums[(size_t)n - 1];   /* 1‑based index */
    return mobius((idx_t)num);
}
