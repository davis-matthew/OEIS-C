/*
 * A161535  Array read by antidiagonals: Row n contains the positive multiples
 *          of the first n primes.
 * https://oeis.org/A161535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element at position n of the array obtained * by
 *            reading the infinite table row‑by‑row; row r contains all *
 *            positive multiples of the first r primes, sorted increasingly. *
 *            The table is scanned by antidiagonals (the sum of indices), and
 *            * a(n) is the n‑th element of that scan
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a small list of primes once and then
 * generates the required row entry by a linear merge of the
 * arithmetic progressions k·p_i.  Complexity is O(r·c) per term,
 * which is well below the limits for n ≤ 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;   /* array of the first primes (uint64_t) */
static size_t prime_cnt = 0;          /* number of primes stored */

/* initialise the prime list once */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;                       /* already initialised */

    uint64_t *tmp = NULL;
    size_t cnt = 0;
    /* 200 000 is enough for n ≤ 10 000 and keeps RAM use small. */
    build_primes_upto(200000, &tmp, &cnt);
    prime_list = tmp;                  /* own the array, free never needed */
    prime_cnt  = cnt;
}

/* return the value of the `col`‑th entry (1‑based) of row `r`
 *   r >= 1, col >= 1.
 * The row contains the union of arithmetic progressions k·p_i
 * (k >= 1, i = 1..r) sorted and duplicate‑free.                 */
static seq_t row_kth_value(idx_t r, idx_t col)
{
    if (r == 0 || col == 0)
        return (seq_t)0;

    init_primes();
    if ((idx_t)prime_cnt < r)
        return (seq_t)0;               /* safety, should not happen */

    size_t npr = (size_t)r;            /* number of primes in this row */
    seq_t *next = (seq_t *)malloc(npr * sizeof(seq_t));

    /* initialise the first multiple of each prime */
    for (size_t i = 0; i < npr; ++i)
        next[i] = (seq_t)prime_list[i];

    seq_t value = 0;
    for (idx_t step = 1; step <= col; ++step) {
        /* find the smallest current multiple (k‑way merge) */
        seq_t m = next[0];
        size_t mi = 0;
        for (size_t i = 1; i < npr; ++i) {
            if (next[i] < m) {
                m = next[i];
                mi = i;
            }
        }
        value = m;                     /* output this element */

        /* advance all primes that contributed this multiple */
        for (size_t i = 0; i < npr; ++i)
            if (next[i] == m)
                next[i] += (seq_t)prime_list[i];
    }
    free(next);
    return value;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* find diagonal d containing position n
     * Diagonal d holds (d‑1) elements.
     * The first diagonal with n is found by trial in O(√n).      */
    idx_t d = 2;
    while (n > d * (d - 1) / 2)
        ++d;

    /* number of elements before this diagonal */
    idx_t prev = (d - 1) * (d - 2) / 2;
    idx_t pos  = n - prev;            /* 1‑based position in this diagonal */

    /* convert to row / column indices (both 1‑based) */
    idx_t row = d - pos;
    idx_t col = pos;

    return row_kth_value(row, col);
}
