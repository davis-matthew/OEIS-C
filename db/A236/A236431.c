/*
 * A236431  Numbers n such that both prime(n)+n and prime(n)-n give a
 *          triangular number.
 * https://oeis.org/A236431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, more
 * Formula:   a(n) = nth index i such that prime(i)+i and prime(i)-i are
 *            triangular numbers
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

static seq_t *indices = NULL;          /* stored indices that satisfy the condition */
static size_t idx_len = 0;             /* number of stored indices */
static size_t idx_cap = 0;             /* allocated capacity of indices */
static idx_t last_checked = 1;         /* next n to check for the condition */

/* test whether a number is triangular: 8*x+1 must be a perfect square */
static bool is_triangular(seq_t x)
{
    seq_t v = x * 8 + 1;
    seq_t r = floor_sqrt(v);
    return r * r == v;
}

/* append a new index to the dynamic array, growing it if necessary */
static void append_index(seq_t val)
{
    if (idx_len == idx_cap) {
        idx_cap = idx_cap == 0 ? 64 : idx_cap * 2;
        seq_t *tmp = (seq_t *)realloc(indices, idx_cap * sizeof(seq_t));
        if (tmp == NULL) /* allocation failure – terminate program */
            abort();
        indices = tmp;
    }
    indices[idx_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; but guard against misuse */

    /* Fill the indices array until it contains at least n elements */
    while (idx_len < n) {
        seq_t p = get_prime(last_checked);        /* prime at position last_checked */
        seq_t plus  = p + last_checked;
        seq_t minus = p - last_checked;
        if (is_triangular(plus) && is_triangular(minus))
            append_index(last_checked);
        last_checked++;
    }

    return indices[n - 1];
}
