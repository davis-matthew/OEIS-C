/*
 * A173816  The sum of the numbers in the n-th antidiagonal of Gilbreath's
 *          array (A036262).
 * https://oeis.org/A173816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{r=1}^{n} A036262(r, n+1-r)
 * Range:     u8 n <= 44, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The n-th antidiagonal of Gilbreath's array a.k.a. A036262 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

static uint64_t *
first_n_primes(idx_t n)
{
    if (n == 0)
        return NULL;
    uint64_t *primes = (uint64_t*)malloc(n * sizeof(uint64_t));
    if (!primes)
        return NULL;
    primes[0] = 2;
    idx_t count = 1;
    uint64_t cand = 3;
    while (count < n) {
        int is_prime = 1;
        for (idx_t i = 0; i < count && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            primes[count++] = cand;
        }
        cand += 2;
    }
    return primes;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Get first n primes */
    uint64_t *primes = first_n_primes(n);
    if (!primes)
        return (seq_t)0;

    /* Allocate two working rows of type seq_t */
    seq_t *row  = seq_alloc_array((size_t)n);
    seq_t *tmp  = seq_alloc_array((size_t)n);
    if (!row || !tmp) {
        free(primes);
        return (seq_t)0;
    }

    /* Initialize first row with the primes */
    for (size_t i = 0; i < (size_t)n; ++i)
        row[i] = (seq_t)primes[i];

    seq_t sum = (seq_t)0;
    size_t len = (size_t)n;

    for (size_t r = 1; r <= (size_t)n; ++r) {
        size_t idx = (size_t)n - r;          /* zero‑based index of the required element */
        sum += row[idx];

        if (r == (size_t)n)
            break;                           /* no further rows */

        /* Compute next row: absolute differences of adjacent elements */
        size_t next_len = len - 1;
        for (size_t i = 0; i < next_len; ++i) {
            seq_t a1 = row[i];
            seq_t a2 = row[i + 1];
            tmp[i] = (a1 > a2) ? (a1 - a2) : (a2 - a1);
        }
        /* Copy back to row for the next iteration */
        for (size_t i = 0; i < next_len; ++i)
            row[i] = tmp[i];
        len = next_len;
    }

    /* Clean up */
    free_seq_array(row);
    free_seq_array(tmp);
    free(primes);

    return sum;
}
