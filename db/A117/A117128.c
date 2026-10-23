/*
 * A117128  Recamán transform of primes (another version): a(0)=1; for n>0,
 *          a(n) = a(n-1) - prime(n) if that number is positive and not
 *          already in the sequence, otherwise a(n) = a(n-1) + prime(n).
 * https://oeis.org/A117128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1995, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán transform of primes (another version)
   a(0) = 1.
   For n>0 let p = prime(n) (1‑based). Then
   a(n) = a(n‑1) – p  if a(n‑1) – p  > 0  and not already in the sequence,
   otherwise a(n) = a(n‑1) + p.

   The sequence is memoised; calls may occur in any order.
   A simple hash‑set of type seq_t is used for membership tests. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_isqrt_u64.h"   /* floor_isqrt_u64() */

static seq_t *seq_cache = NULL;   /* array of terms (a[0] …) */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t seq_cap   = 0;      /* allocated capacity           */

struct hashset {
    seq_t *keys;      /* 0 value indicates empty slot   */
    size_t cap;       /* power of two                  */
    size_t used;      /* number of occupied slots      */
};
static struct hashset used_set = { NULL, 0, 0 };

/* Hash for seq_t (works for 8‑, 16‑, 32‑, 64‑ and 128‑bit values). */
static inline size_t hash_seq_t(seq_t v)
{
#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ == 16
    return (size_t)(((__uint128_t)v) * 11995408973635179863ULL);
#else
    return (size_t)((v) * 11995408973635179863ULL);
#endif
}

/* Ensure the hash set has enough capacity
   (next power of two that can hold the needed number of keys). */
static void ensure_hashset_cap(size_t needed)
{
    if (used_set.cap == 0) {
        used_set.cap = 8;
        used_set.keys = (seq_t *)calloc(used_set.cap, sizeof(seq_t));
    }
    while (used_set.cap < needed * 2) {
        size_t new_cap = used_set.cap << 1;
        seq_t *new_keys = (seq_t *)calloc(new_cap, sizeof(seq_t));

        /* rehash existing keys */
        for (size_t i = 0; i < used_set.cap; ++i) {
            seq_t k = used_set.keys[i];
            if (k) {
                size_t h = hash_seq_t(k) & (new_cap - 1);
                while (new_keys[h]) h = (h + 1) & (new_cap - 1);
                new_keys[h] = k;
            }
        }
        free(used_set.keys);
        used_set.keys = new_keys;
        used_set.cap  = new_cap;
    }
}

/* Insert a key into the hash set (duplicates are ignored). */
static void hashset_insert(seq_t k)
{
    if (k == 0) return;          /* 0 never occurs in this sequence */
    ensure_hashset_cap(used_set.used + 1);
    size_t h = hash_seq_t(k) & (used_set.cap - 1);
    while (used_set.keys[h]) {
        if (used_set.keys[h] == k) return;   /* already present */
        h = (h + 1) & (used_set.cap - 1);
    }
    used_set.keys[h] = k;
    ++used_set.used;
}

/* Check whether a key is present in the hash set. */
static bool hashset_contains(seq_t k)
{
    if (k == 0) return false;
    size_t h = hash_seq_t(k) & (used_set.cap - 1);
    while (used_set.keys[h]) {
        if (used_set.keys[h] == k) return true;
        h = (h + 1) & (used_set.cap - 1);
    }
    return false;
}

/* ----------------------------------------------------------------
   Prime number storage and generation.
   We keep a simple dynamic array of 32-bit primes.
   ---------------------------------------------------------------- */
static uint32_t *prime_arr = NULL;   /* prime_arr[0] = 2, prime_arr[1] = 3, … */
static size_t   prime_cnt = 0;      /* number of primes stored      */
static size_t   prime_cap = 0;      /* allocated capacity           */

/* Ensure at least n primes are available (1‑based indexing). */
static void ensure_primes(size_t n)
{
    if (prime_cnt >= n) return;

    if (prime_cap == 0) {
        prime_cap = 256;
        prime_arr = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    }

    uint32_t candidate = (prime_cnt == 0) ? 2 : prime_arr[prime_cnt - 1] + 1;

    while (prime_cnt < n) {
        if (candidate < 2) candidate = 2;
        uint64_t lim   = floor_isqrt_u64(candidate);
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && prime_arr[i] <= lim; ++i) {
            if (candidate % prime_arr[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                prime_arr = (uint32_t *)realloc(prime_arr, prime_cap * sizeof(uint32_t));
            }
            prime_arr[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* ----------------------------------------------------------------
   Sequence cache management.
   ---------------------------------------------------------------- */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap == 0) {
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    while (seq_cap < needed) {
        seq_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
}

static void append_seq_term(seq_t val)
{
    ensure_seq_cap(seq_len + 1);
    seq_cache[seq_len++] = val;
    hashset_insert(val);
}

/* ----------------------------------------------------------------
   Main function requested by the problem.
   ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Handle the first term. */
    if (n == 0) return (seq_t)1;

    /* Return cached value if it already exists. */
    if (seq_len > (size_t)n) return seq_cache[n];

    /* Initialise cache on first call. */
    if (seq_len == 0) {
        seq_cap   = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len   = 1;
        seq_cache[0] = (seq_t)1;
        hashset_insert((seq_t)1);
    }

    /* We need primes up to index n (1‑based). */
    ensure_primes((size_t)n);

    /* Compute successive terms until we reach index n. */
    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        seq_t prev = seq_cache[i - 1];
        uint32_t p = prime_arr[i - 1];           /* prime(i), 1‑based */

        seq_t cand = prev + (seq_t)p;            /* default addition */
        if (prev >= (seq_t)p) {
            seq_t tmp = prev - (seq_t)p;         /* subtraction candidate */
            if (tmp > 0 && !hashset_contains(tmp)) {
                cand = tmp;                      /* use subtraction */
            }
        }
        append_seq_term(cand);
    }

    return seq_cache[n];
}
