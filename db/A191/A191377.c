/*
 * A191377  Primes that are the sum of a positive Fibonacci number and the
 *          squares of two positive Fibonacci numbers.
 * https://oeis.org/A191377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p for which there exist positive Fibonacci
 *            numbers F_i, F_j, F_k with p = F_i + F_j^2 + F_k^2
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* maximum number of primes to store (enough for n up to ~10000) */
#define MAX_PRIMES 20000
/* maximum Fibonacci index stored (enough for 64‑bit primes) */
#define MAX_FIB 100

/* static cache for primes */
static seq_t prime_cache[MAX_PRIMES];
static size_t prime_count = 0;
static idx_t next_candidate = 2;

/* static cache for Fibonacci numbers */
static seq_t fib_cache[MAX_FIB];
static size_t fib_len = 0;

/* linear search for the last Fibonacci number contains in cache */
static void ensure_fib_cache(idx_t limit)
{
    if (fib_len == 0) {
        fib_cache[0] = 0;
        fib_cache[1] = 1;
        fib_len = 2;
    }
    /* grow Fibonacci cache up to limit or up to the maximum allowed */
    while (fib_len < MAX_FIB && fib_cache[fib_len - 1] <= limit) {
        fib_cache[fib_len] = fib_cache[fib_len - 1] + fib_cache[fib_len - 2];
        fib_len++;
    }
}

/* binary search to test if x is a Fibonacci number in the cache */
static bool is_fib_value(seq_t x)
{
    if (x < fib_cache[1] || fib_len == 0) return false;
    size_t lo = 1, hi = fib_len - 1;
    while (lo <= hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (fib_cache[mid] == x) return true;
        if (fib_cache[mid] < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return false;
}

/* test whether prime p can be expressed as F_i + F_j^2 + F_k^2 */
static bool has_representation(seq_t p)
{
    for (size_t j = 1; j < fib_len; ++j) {
        seq_t sq_j = fib_cache[j] * fib_cache[j];
        if (sq_j >= p) break;
        for (size_t k = j; k < fib_len; ++k) {
            seq_t sq_k = fib_cache[k] * fib_cache[k];
            seq_t sumsq = sq_j + sq_k;
            if (sumsq >= p) break;
            seq_t rem = p - sumsq;
            if (is_fib_value(rem)) return true;
        }
    }
    return false;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* generate primes until we have at least n valid terms */
    while (prime_count < (size_t)n) {
        seq_t cur_seq = (seq_t)next_candidate;
        if (is_prime_big(cur_seq)) {
            ensure_fib_cache(next_candidate);       /* extend fib cache */
            if (has_representation(cur_seq)) {
                /* store the prime */
                if (prime_count < MAX_PRIMES)
                    prime_cache[prime_count++] = cur_seq;
            }
        }
        /* move to next odd candidate except when at 2 */
        if (next_candidate <= 2) {
            next_candidate = 3;
        } else {
            next_candidate += 2;
        }
    }
    /* return the n-th term (1‑based) */
    return prime_cache[n - 1];
}
