/*
 * A023608  Convolution of Fibonacci numbers and (1, prime(1), prime(2), ...).
 * https://oeis.org/A023608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} Fib(k+1) * Prime(n-k), * with Prim(0)=1,
 *            Prime(1)=2, Prime(2)=3, ... (OEIS A023608)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached Fibonacci values: fib_cache[i] = Fib(i+1)  (i = 0 ..) */
static seq_t *fib_cache = NULL;
static size_t fib_len = 0;

/* Cached prime values: prime_vals[i] = Prime(i),  prime_vals[0] == 1 */
static idx_t *prime_vals = NULL;
static size_t prime_len = 0;

/* Cached sequence values: a_cache[i] = a(i) */
static seq_t *a_cache = NULL;
static size_t a_len = 0;

/* Ensure Fibonacci cache holds values up to index n (inclusive) */
static void ensure_fib(size_t n)
{
    if (fib_len == 0) {
        fib_cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!fib_cache) abort();
        fib_cache[0] = (seq_t)1;   /* Fib(1) */
        fib_cache[1] = (seq_t)1;   /* Fib(2) */
        fib_len = 2;
    }
    while (fib_len <= n) {
        seq_t next = fib_cache[fib_len - 1] + fib_cache[fib_len - 2];
        seq_t *tmp = (seq_t *)realloc(fib_cache, (fib_len + 1) * sizeof(seq_t));
        if (!tmp) abort();
        fib_cache = tmp;
        fib_cache[fib_len] = next;
        ++fib_len;
    }
}

/* Ensure prime values up to index n (inclusive).
 * prime_vals[0] is 1 by definition, prime_vals[1] = 2, etc. */
static void ensure_primes(size_t n)
{
    if (prime_len == 0) {
        prime_vals = (idx_t *)malloc(2 * sizeof(idx_t));
        if (!prime_vals) abort();
        prime_vals[0] = (idx_t)1;   /* Prime(0) = 1 by convention */
        prime_len = 1;
    }
    idx_t candidate = prime_vals[prime_len - 1] + 1;
    while (prime_len <= n) {
        if (candidate < 2) candidate = 2;
        bool is_prime = true;
        for (size_t i = 1; i < prime_len; ++i) {
            idx_t p = prime_vals[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            idx_t *tmp = (idx_t *)realloc(prime_vals, (prime_len + 1) * sizeof(idx_t));
            if (!tmp) abort();
            prime_vals = tmp;
            prime_vals[prime_len] = candidate;
            ++prime_len;
        }
        ++candidate;
    }
}

/* Ensure a_cache holds values up to index n (inclusive) */
static void ensure_a(size_t n)
{
    if (a_len == 0) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        a_len = 1;
    }
    if (a_len > n) return;   /* already computed */

    seq_t *tmp = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;

    for (size_t m = a_len; m <= n; ++m) {
        ensure_fib(m);
        ensure_primes(m);
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k <= m; ++k) {
            seq_t fib_val = fib_cache[k];              /* Fib(k+1) */
            seq_t prime_val = (seq_t)prime_vals[m - k];/* Prime(m-k) */
            sum += fib_val * prime_val;
        }
        a_cache[m] = sum;
    }
    a_len = n + 1;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_a(idx);
    return a_cache[idx];
}
