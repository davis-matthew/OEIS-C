/*
 * A109182  Numbers k such that the k-th and (k+1)-th primes have the same sum
 *          of squares of digits.
 * https://oeis.org/A109182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the index k such that the k‑th and (k+1)-th primes have
 *            identical sum of squares of their decimal digits. The function
 *            generates primes on demand, caches the indices, and returns the
 *            n‑th such index (1‑based)
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_squaresum, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_malloc.h"
#include "../utils/digit_squaresum.h"

/* Simple deterministic primality test for uint64_t, sufficient for
   the primes needed up to n≈10000. */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Static storage for sequence values (indices k). */
static seq_t *seq_vals   = NULL;
static idx_t  seq_cap    = 0;
static idx_t  seq_size   = 0;

/* Static state for prime generation. */
static bool    primed     = false;
static uint64_t prev_prime = 0;
static idx_t   prev_index  = 0;
static uint64_t candidate  = 2;

/* Ensure buffer large enough for at least N elements. */
static void ensure_buffer(idx_t N)
{
    if (!seq_vals) {
        seq_cap = N > 10000 ? N : 10000;
        seq_vals = seq_malloc(seq_cap);
        if (!seq_vals) abort();
        seq_size = 0;
    } else if (N > seq_cap) {
        idx_t new_cap = seq_cap * 2;
        while (new_cap < N) new_cap *= 2;
        seq_t *tmp = seq_malloc(new_cap);
        if (!tmp) abort();
        for (idx_t i = 0; i < seq_size; ++i) tmp[i] = seq_vals[i];
        free(seq_vals);
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
}

/* Append a new value to seq_vals. */
static void append_value(idx_t val)
{
    if (seq_size >= seq_cap) abort();
    seq_vals[seq_size++] = (seq_t)val;
}

/* Main function: returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but safe */
    if (!primed) {
        primed     = true;
        prev_prime = 0;
        prev_index = 0;
        candidate  = 2;
    }
    ensure_buffer(n);
    while (seq_size < n) {
        uint64_t p = candidate;
        if (p > 2 && (p & 1) == 0) {
            p++;                      /* skip even after 2 */
            candidate = p;
        }
        if (is_prime(p)) {
            idx_t p_idx = prev_index + 1;
            if (prev_prime != 0 &&
                digit_squaresum(prev_prime) == digit_squaresum(p)) {
                append_value(prev_index);
            }
            prev_prime = p;
            prev_index = p_idx;
        }
        candidate = (p == 2) ? 3 : p + 2;   /* next candidate */
    }
    return seq_vals[n-1];
}
