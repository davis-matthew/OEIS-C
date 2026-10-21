/*
 * A225035  Primes such that there is a nontrivial rearrangement of the digits
 *          which is a prime.
 * https://oeis.org/A225035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1405, u32 n <= 1405, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, is_prime_big, sort_bytes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: primes whose digits can be
   rearranged into another prime (non‑trivial permutation, leading
   zeros allowed). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"
#include "../utils/get_digits.h"
#include "../utils/sort_bytes.h"

/* Dynamic array of results. */
static seq_t  *seq_arr = NULL;   /* collected sequence values */
static size_t seq_cap = 0, seq_len = 0;

/* Dynamic array of all primes seen when generating next_prime. */
static uint64_t *prime_arr = NULL;
static size_t   prime_cap = 0, prime_cnt = 0;

/* Ensure a capacity for the result array. */
static void seq_ensure_capacity(size_t needed) {
    if (needed <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < needed) newcap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* Ensure a capacity for the prime list. */
static void prime_ensure_capacity(size_t needed) {
    if (needed <= prime_cap) return;
    size_t newcap = prime_cap ? prime_cap * 2 : 16;
    while (newcap < needed) newcap <<= 1;
    prime_arr = (uint64_t *)realloc(prime_arr, newcap * sizeof(uint64_t));
    prime_cap = newcap;
}

/* Proper lexicographic next permutation for arrays of uint8_t. */
static bool next_permutation(uint8_t *a, size_t n) {
    if (n < 2) return false;
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i+1]) i--;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (a[j] <= a[i]) j--;
    uint8_t tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    /* reverse a[i+1 .. n-1] */
    int l = i + 1, r = (int)n - 1;
    while (l < r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
        l++; r--;
    }
    return true;
}

/* Check whether the prime p has a non‑trivial prime permutation. */
static bool has_prime_permutation(seq_t p) {
    uint8_t digits[32];
    size_t dlen;
    get_digits(p, digits, &dlen);

    /* Copy and sort so we start from the smallest permutation. */
    uint8_t perm[32];
    for (size_t i = 0; i < dlen; ++i) perm[i] = digits[i];
    sort_bytes(perm, dlen);

    seq_t val;
    do {
        val = 0;
        for (size_t i = 0; i < dlen; ++i)
            val = val * (seq_t)10 + (seq_t)perm[i];
        if (val != p && is_prime_big(val)) return true;
    } while (next_permutation(perm, dlen));

    return false;
}

/* Generate the next prime using trial division and stored primes. */
static uint64_t next_prime(void) {
    uint64_t cand = (prime_cnt == 0) ? 2ULL : prime_arr[prime_cnt-1] + 1;
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t pr = prime_arr[i];
            if (pr * pr > cand) break;
            if (cand % pr == 0) { ok = false; break; }
        }
        if (ok) {
            prime_ensure_capacity(prime_cnt + 1);
            prime_arr[prime_cnt++] = cand;
            return cand;
        }
        ++cand;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; behavior below is irrelevant */

    /* Generate primes until we have at least n qualifying terms. */
    while (seq_len < (size_t)n) {
        uint64_t p64 = next_prime();
        seq_t p = (seq_t)p64;
        if (has_prime_permutation(p)) {
            seq_ensure_capacity(seq_len + 1);
            seq_arr[seq_len++] = p;
        }
    }
    return seq_arr[(size_t)n - 1];
}
