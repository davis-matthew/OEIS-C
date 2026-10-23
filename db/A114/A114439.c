/*
 * A114439  Indices of semiprime pentagonal numbers.
 * https://oeis.org/A114439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that the pentagonal number k*(3*k-1)/2 is a
 *            product of exactly two primes
 * Range:     u8 n <= 29, u16 n <= 1672, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation iteratively tests integers k for the semiprime property of P(k).  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

#ifdef __cplusplus
#include <vector>
#endif

/* Helper to add a value to the dynamic list */
#if defined(__cplusplus)
static std::vector<seq_t> S;
static void add_val(seq_t v) { S.push_back(v); }
static size_t get_size(void) { return S.size(); }
#else
static seq_t *S = NULL;
static size_t S_size = 0;
static size_t S_cap = 0;
static void add_val(seq_t v) {
    if (S_size == S_cap) {
        size_t new_cap = S_cap ? S_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        S = tmp;
        S_cap = new_cap;
    }
    S[S_size++] = v;
}
static size_t get_size(void) { return S_size; }
#endif

/* Main function: returns the n-th index of a semiprime pentagonal number */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based.  For n=0 we return 0 (undefined). */
    if (n == 0) return 0;

    static idx_t next_k = 1;               /* next k to test                      */
    /* Keep generating until we have at least n entries. */
    while (get_size() < n) {
        if (next_k & 1ULL) {
            /* k is odd: P(k) = k * ((3k-1)/2)   */
            if (is_prime((uint64_t)next_k) &&
                is_prime((3ULL * next_k - 1) / 2ULL)) {
                add_val((seq_t)next_k);
            }
        } else {
            /* k is even: P(k) = (k/2) * (3k-1)   */
            if (is_prime(next_k / 2ULL) &&
                is_prime(3ULL * next_k - 1ULL)) {
                add_val((seq_t)next_k);
            }
        }
        ++next_k;
    }
    /* Return the n-th term (1‑based). */
    return S[n - 1];
}
