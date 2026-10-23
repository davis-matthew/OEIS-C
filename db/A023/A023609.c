/*
 * A023609  Convolution of Fibonacci numbers and composite numbers.
 * https://oeis.org/A023609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} Fib(k+1) * C_{n-k}, where Fib(0)=0, Fib(1)=1,
 *            and C(j) is the j‑th composite number with C(0)=4, C(1)=6,
 *            C(2)=8, …
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cached Fibonacci values */
static seq_t *fib = NULL;
static size_t fib_cap = 0;   /* capacity of fib array   */
static size_t fib_len = 0;   /* next free index        */

/* Cached composite numbers (uint64_t) */
static uint64_t *composite = NULL;
static size_t comp_cap = 0;  /* capacity of composite array   */
static size_t comp_len = 0;  /* next free index               */
static uint64_t next_candidate = 4;   /* next number to test for composite */

/* Ensure fib array contains values up to index n+1 (fib[n+1] needed) */
static void ensure_fib_size(idx_t n)
{
    /* we need fib[n+1], i.e. fib_len > n+1 */
    while (fib_len <= (size_t)n + 1) {
        if (fib_len >= fib_cap) {
            size_t new_cap = fib_cap == 0 ? 4 : fib_cap * 2;
            seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);          /* abort on memory failure */
            fib = tmp;
            fib_cap = new_cap;
        }
        if (fib_len == 0) {
            /* initialise first two Fibonacci numbers */
            fib[0] = 0;
            fib[1] = 1;
            fib_len = 2;
        } else {
            fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
            ++fib_len;
        }
    }
}

/* Ensure composite array contains at least n+1 composites (index n) */
static void ensure_comp_size(idx_t n)
{
    while (comp_len <= (size_t)n) {
        if (comp_len >= comp_cap) {
            size_t new_cap = comp_cap == 0 ? 16 : comp_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(composite, new_cap * sizeof(uint64_t));
            if (!tmp) exit(1);
            composite = tmp;
            comp_cap = new_cap;
        }
        /* find the next composite number */
        while (true) {
            if (!is_prime((idx_t)next_candidate)) {
                composite[comp_len] = next_candidate;
                ++comp_len;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_fib_size(n);
    ensure_comp_size(n);

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = fib[k + 1] * (seq_t)composite[n - k];
        sum += term;
    }
    return sum;
}
