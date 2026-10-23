/*
 * A111325  Numbers k such that 11 divides prime(1) + ... + prime(k).
 * https://oeis.org/A111325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 6100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A111325
   Non‑nonnumerical sequence:
   Numbers k such that 11 divides prime(1) + … + prime(k).
   a(1) = 8, a(2) = 17, …
   The function computes the index k directly.
   It generates primes on demand, keeps a running sum modulo 11,
   and records the positions where the sum is a multiple of 11.   */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Dynamic array of found primes (uint64_t).  */
static uint64_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_cnt = 0;

/* Dynamic array of indices k where the cumulative prime sum is divisible by 11. */
static idx_t *indices = NULL;
static size_t indices_cap = 0;
static size_t indices_cnt = 0;

/* Candidate to test for primality.  */
static uint64_t next_candidate = 2;

/* Current cumulative sum of primes modulo 11 (0..10).  */
static int cumulative_mod = 0;

/* Append a prime value to the primes array.  */
static void push_prime(uint64_t val)
{
    if (primes_cnt >= primes_cap) {
        size_t new_cap = (primes_cap ? primes_cap * 2 : 16);
        uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = val;
}

/* Append an index k to the indices array.  */
static void push_index(idx_t idx)
{
    if (indices_cnt >= indices_cap) {
        size_t new_cap = (indices_cap ? indices_cap * 2 : 16);
        idx_t *tmp = (idx_t*)realloc(indices, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        indices = tmp;
        indices_cap = new_cap;
    }
    indices[indices_cnt++] = idx;
}

/* Ensure that at least `n` indices have been found.  */
static void ensure_n(idx_t n)
{
    if (indices_cnt >= n) return;

    while (indices_cnt < n) {
        uint64_t cand = next_candidate;
        bool prime = true;

        if (cand < 2) {
            prime = false;
        } else {
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > cand) break;
                if (cand % p == 0) { prime = false; break; }
            }
        }

        if (prime) {
            push_prime(cand);
            cumulative_mod = (cumulative_mod + (int)(cand % 11)) % 11;
            if (cumulative_mod == 0) {
                /* primes_cnt is 1‑based index of this prime.  */
                push_index(primes_cnt);
            }
        }

        next_candidate = cand + 1;
    }
}

/* ------------------------------------------------------------------ */
/* External function required by the test harness.                     */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for offset 1 – harmless fallback. */
    ensure_n(n);
    return (seq_t)indices[n - 1];
}
