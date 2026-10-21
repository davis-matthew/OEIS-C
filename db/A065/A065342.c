/*
 * A065342  Triangle of sum of two primes: prime(n)+prime(k) with n >= k >= 1.
 * https://oeis.org/A065342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = prime(row(n)) + prime(col(n)) where (row, col) is the
 *            anti‑diagonal coordinate of position n in the triangle
 *            1‑indexed, and prime(k) is the k‑th prime (k ≥ 1)
 * Range:     u8 n <= 71, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Local helpers for prime generation. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static storage for the list of primes discovered so far. */
static idx_t *prime_list = NULL;   /* 0‑based array of primes */
static size_t prime_cnt  = 0;      /* number of stored primes      */
static size_t prime_cap  = 0;      /* allocated capacity           */

/* Ensure that prime_list contains at least “needed” primes. */
static void ensure_primes(size_t needed)
{
    if (prime_cnt >= needed)
        return;

    /* Grow the array if necessary. */
    size_t new_cap = prime_cap ? prime_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    idx_t *new_arr = (idx_t *)malloc(new_cap * sizeof(idx_t));
    if (!new_arr)
        abort();   /* memory failure – unlikely for expected sizes */

    if (prime_list) {
        memcpy(new_arr, prime_list, prime_cnt * sizeof(idx_t));
        free(prime_list);
    }
    prime_list = new_arr;
    prime_cap  = new_cap;

    /* Build missing primes. */
    if (prime_cnt == 0) {
        prime_list[prime_cnt++] = 2;
    }

    /* Start from the appropriate first candidate. */
    idx_t candidate = (prime_cnt == 1) ? 3 : prime_list[prime_cnt - 1] + 2;

    while (prime_cnt < needed) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p > candidate / p)   /* p * p > candidate (safe) */
                break;
            if ((candidate % p) == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = candidate;
        }
        candidate += 2;   /* skip even numbers */
    }
}

/* Return the r‑th triangular number, T(r) = r(r+1)/2, in seq_t. */
static inline seq_t triangular(idx_t r)
{
    return ((seq_t)r * (r + 1)) / 2;
}

/* Triangle of sum of two primes */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* Find the row containing position n via binary search on triangular numbers. */
    idx_t low  = 1;
    idx_t high = 1;
    while (triangular(high) < n)
        high <<= 1;

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) >= n)
            high = mid;
        else
            low  = mid;
    }
    idx_t row = high;                           /* row index (1‑based) */
    seq_t t_prev = triangular(row - 1);          /* previous triangular number */
    idx_t col = (idx_t)((seq_t)n - t_prev);      /* column index (1‑based) */

    /* Ensure enough primes are available. */
    if (row > prime_cnt)
        ensure_primes(row);

    seq_t sum = (seq_t)prime_list[row - 1] + (seq_t)prime_list[col - 1];
    return sum;
}
