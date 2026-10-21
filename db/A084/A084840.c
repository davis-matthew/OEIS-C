/*
 * A084840  Write the numbers 1, 2, ... in a triangle with n terms in the n-th
 *          row; a(n) = number of abundant integers in n-th row.
 * https://oeis.org/A084840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A091194( n*(n+1)/2 ) – A091194( (n−1)*n/2 ) where
 *            A091194(m) is the number of abundant integers ≤ m
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state for incremental sieve */
static size_t max_T_computed = 0;   /* Largest triangular number processed. */
static uint32_t *sigma = NULL;      /* Sum of divisors up to max_T_computed. */
static uint32_t *cumulative = NULL;/* Prefix counts of abundant numbers. */

/* --------------------------------- helpers --------------------------------- */

static inline seq_t triangle(idx_t k)
{
    return (seq_t)k * ((seq_t)k + 1) / 2;
}

/* Ensure data is available up to triangular(n). */
static void ensure_up_to(idx_t n)
{
    seq_t new_T_seq = triangle(n);
    size_t new_T = (size_t)new_T_seq;

    if (new_T <= max_T_computed)
        return;                  /* already computed */

    /* --------- grow sigma array --------- */
    if (!sigma) {
        sigma = (uint32_t*)malloc((new_T + 1) * sizeof(uint32_t));
        if (!sigma) exit(1);
        for (size_t i = 0; i <= new_T; ++i) sigma[i] = 0;
    } else {
        uint32_t *tmp = (uint32_t*)realloc(sigma, (new_T + 1) * sizeof(uint32_t));
        if (!tmp) exit(1);
        sigma = tmp;
        /* new slots are zero by realloc but be explicit */
        for (size_t i = max_T_computed + 1; i <= new_T; ++i) sigma[i] = 0;
    }

    /* --------- grow cumulative array --------- */
    if (!cumulative) {
        cumulative = (uint32_t*)malloc((new_T + 1) * sizeof(uint32_t));
        if (!cumulative) exit(1);
        cumulative[0] = 0;
    } else {
        uint32_t *tmp = (uint32_t*)realloc(cumulative, (new_T + 1) * sizeof(uint32_t));
        if (!tmp) exit(1);
        cumulative = tmp;
    }

    /* --------- incremental divisor‑sum sieve --------- */
    /* contributions from new numbers and from new multiples of existing divisors */
    for (size_t i = 1; i <= new_T; ++i) {
        /* first multiple of i that is > max_T_computed */
        size_t start = (max_T_computed + i) / i * i;
        if (start <= max_T_computed)
            start += i;
        if (start < i) start = i;
        for (size_t j = start; j <= new_T; j += i)
            sigma[j] += (uint32_t)i;
    }

    /* --------- build cumulative counts --------- */
    for (size_t i = max_T_computed + 1; i <= new_T; ++i) {
        bool abundant = sigma[i] > 2 * (uint32_t)i;
        cumulative[i] = cumulative[i - 1] + (abundant ? 1 : 0);
    }

    max_T_computed = new_T;
}

/* -------------------------------- main sequence -------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined below offset */

    seq_t T_n   = triangle(n);
    seq_t T_prev = T_n - n;        /* triangle(n-1) */

    ensure_up_to(n);               /* guarantees arrays up to T_n */

    size_t idx_n   = (size_t)T_n;
    size_t idx_prev = (size_t)T_prev;

    return (seq_t)(cumulative[idx_n] - cumulative[idx_prev]);
}
