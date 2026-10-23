/*
 * A181393  Numbers of the form Fibonacci(2^c)/Fibonacci(2^b), 1 <= b < c.
 * https://oeis.org/A181393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ∏_{i = k-r}^{k} P_i
 * where P_1 = 3 and P_i = P_{i-1}^2 – 2  (i ≥ 2),
 *   k is the smallest integer with k(k+1)/2 ≥ n,
 *   r = n – k(k‑1)/2 – 1  (0 ≤ r < k).
 * This enumerates the numbers F(2^c)/F(2^b) for 1 ≤ b < c
 * in the order used by OEIS A181393:  P_k, P_{k-1}P_k, ..., P_1…P_k.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"

static seq_t *primes = NULL;     /* 1‑based array: primes[i] = P_i   */
static idx_t primes_used = 0;    /* highest i for which primes[i] exists */

/* Ensure that primes[1..k] are computed. */
static void ensure_primes(idx_t k)
{
    if (primes_used >= k)
        return;

    size_t new_cap = (primes_used == 0 ? 1 : primes_used * 2);
    while (new_cap < (size_t)(k + 1))
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* OOM – the problem statement assumes enough memory */
    primes = tmp;

    for (idx_t i = primes_used + 1; i <= k; ++i) {
        if (i == 1) {
            primes[i] = (seq_t)3;                            /* P_1 = 3 */
        } else {
            seq_t sq = seq_mul(primes[i - 1], primes[i - 1]);/* (P_{i-1})^2   */
            primes[i] = sq - (seq_t)2;                       /* P_i = sq - 2 */
        }
    }
    primes_used = k;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;           /* undefined, but avoids UB */

    /* Find the block index k such that k*(k+1)/2 ≥ n */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n)
        ++k;

    idx_t t_prev = (k - 1) * k / 2;          /* terms before block k */
    idx_t r = n - t_prev - 1;               /* offset inside block (0‑based) */

    ensure_primes(k);                       /* make sure all needed P_i exist */

    seq_t result = (seq_t)1;
    for (idx_t i = k - r; i <= k; ++i) {
        result = seq_mul(result, primes[i]); /* multiply last r+1 primes */
    }
    return result;
}
