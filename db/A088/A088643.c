/*
 * A088643  Triangle read by rows: row n >= 1 is obtained as follows. Start
 *          with n, next term is always largest number m with 1 <= m < n which
 *          has not yet appeared in that row and such that m + previous term
 *          in the row is a prime. Stop when no further m can be found.
 * https://oeis.org/A088643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 9730, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088643
   Computes terms of the triangle read by rows, where each row
   starts with its row number n and subsequently takes the largest
   unused integer m (< n) such that m + previous_term is prime.
   The sequence a(n) is the flattened triangle, offset 1. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* Static state shared by all calls */
static seq_t *a_seq      = NULL;   /* array of computed sequence values   */
static size_t a_len      = 0;      /* number of values stored             */
static size_t a_cap      = 0;      /* allocated capacity of a_seq         */
static idx_t  next_row   = 1;      /* next row number to generate         */

static uint8_t *prime_arr = NULL;   /* boolean array of primes (1 = prime) */
static size_t  prime_lim = 0;      /* largest index that prime_arr is valid for */

/* Ensure prime_arr contains prime information up to new_lim (inclusive) */
static void ensure_prime_limit(size_t new_lim)
{
    if (new_lim <= prime_lim) return;

    size_t old_lim = prime_lim;
    size_t sz = new_lim + 1;
    uint8_t *new_arr = (uint8_t*)malloc(sz);
    if (!new_arr) exit(1);              /* allocation failure -> abort      */
    for (size_t i = 0; i < sz; ++i) new_arr[i] = 0;
    new_arr[2] = 1;                     /* 2 is prime                         */
    for (size_t i = 3; i <= new_lim; ++i) new_arr[i] = 1;

    size_t sq = (size_t)sqrt((double)new_lim);
    for (size_t i = 2; i <= sq; ++i)
        if (new_arr[i])
            for (size_t j = i * i; j <= new_lim; j += i)
                new_arr[j] = 0;

    free(prime_arr);
    prime_arr = new_arr;
    prime_lim = new_lim;
}

/* Append a single value to the global sequence array, growing if needed */
static void append_value(seq_t val)
{
    if (a_len == a_cap) {
        size_t new_cap = (a_cap == 0) ? 32 : a_cap * 2;
        seq_t *new_arr = (seq_t*)realloc(a_seq, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        a_seq = new_arr;
        a_cap = new_cap;
    }
    a_seq[a_len++] = val;
}

/* Generate the next row of the triangle, starting with 'row_num' */
static void generate_row(idx_t row_num)
{
    /* Ensure we have prime data up to 2*row_num */
    ensure_prime_limit((size_t)row_num * 2);

    /* local 'used' array for numbers 1 .. row_num-1 */
    unsigned char *used = (unsigned char*)calloc((size_t)row_num, 1);
    if (!used) exit(1);

    seq_t prev = row_num;
    append_value(prev);

    idx_t remaining = row_num - 1;
    while (remaining > 0) {
        int candidate = -1;
        for (int m = (int)row_num - 1; m >= 1; --m) {
            if (!used[m] && prime_arr[(size_t)(prev + m)]) {
                candidate = m;
                break;
            }
        }
        if (candidate == -1) break;   /* no suitable number found */

        used[(size_t)candidate] = 1;
        prev = candidate;
        append_value(prev);
        --remaining;
    }

    free(used);
}

/* Public interface required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; value unused for n==0   */
    while (a_len < n) {
        generate_row(next_row++);
    }
    return a_seq[n - 1];
}
