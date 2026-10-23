/*
 * A111131  Least cube greater than its predecessor such that their difference
 *          is a prime or a prime multiplied by a power of two.
 * https://oeis.org/A111131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   cube, isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A111131:  a(n) = the least cube > a(n‑1) such that their difference
   is a prime or a prime multiplied by a power of two.
   The recurrence is implemented by memoisation with an auxiliary table
   of cube roots.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/isprime_big.h"   /* isprime_big(seq_t n) */
#include "../utils/cube.h"          /* cube(seq_t x) */

/* Global memoisation tables */
static seq_t *seq  = NULL;   /* cached a(i) values */
static idx_t *root = NULL;   /* cached cube roots r such that a(i)=r^3 */
static idx_t len = 0;        /* number of entries currently cached       */
static idx_t cap = 0;        /* current capacity of the tables           */

/* Test whether a difference diff is a prime or prime·2^k */
static bool valid_diff(seq_t diff)
{
    seq_t d   = diff;
    const seq_t two = (seq_t)2;
    /* strip factors of 2 */
    while (d % two == (seq_t)0)
        d /= two;
    return d > (seq_t)1 && isprime_big(d);
}

/* Ensure the memoisation tables can store up to n entries */
static void build_up_to(idx_t n)
{
    if (len >= n) return;

    /* initialise / grow tables */
    if (cap == 0) {
        cap  = 16;
        seq  = (seq_t *)malloc(cap * sizeof(seq_t));
        root = (idx_t *)malloc(cap * sizeof(idx_t));
    }
    while (cap < n) {
        cap <<= 1;
        seq  = (seq_t *)realloc(seq,  cap * sizeof(seq_t));
        root = (idx_t *)realloc(root, cap * sizeof(idx_t));
    }

    /* seed first term */
    if (len == 0) {
        seq[0]  = (seq_t)1;   /* a(1) = 1 */
        root[0] = (idx_t)1;   /* cube root of 1 */
        len    = 1;
    }

    /* compute subsequent terms */
    for (idx_t i = len; i < n; ++i) {
        idx_t r = root[i-1] + 1;   /* start just above previous root */
        for (;;) {
            seq_t c   = cube((seq_t)r);
            seq_t diff = c - seq[i-1];
            if (valid_diff(diff)) {
                seq[i]  = c;
                root[i] = r;
                break;
            }
            ++r;
        }
    }
    len = n;
}

/* Public accessor */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */
    build_up_to(n);
    return seq[n-1];
}
