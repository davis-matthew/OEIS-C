/*
 * A228234  Number of strict Gaussian primes of norm less than or equal to n
 *          in the first quadrant on or below the first diagonal.
 * https://oeis.org/A228234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p ≤ n² such that p = 2 or p ≡ 1 (mod 4)
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts strict Gaussian primes (a+bi, a>0,b>0,b≤a) of norm ≤ n. */
/* The function uses an incremental sieve up to n². */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_bit.h"

/* Static state for incremental sieve and prime lists */
static uint8_t *sieve = NULL;          /* bitset for odd composites */
static idx_t sieve_limit = 0;          /* highest number sieved (inclusive) */

static uint32_t *primes_all = NULL;    /* all primes found */
static size_t primes_all_len = 0, primes_all_cap = 0;

static uint32_t *special_primes = NULL;/* primes 2 or ≡1 (mod 4) */
static size_t special_len = 0, special_cap = 0;

/* Ensure dynamic array capacity */
static void ensure_cap(size_t *len, size_t *cap, uint32_t **arr)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : *cap << 1;
        uint32_t *new_arr = (uint32_t *)realloc(*arr, new_cap * sizeof(uint32_t));
        if (!new_arr) abort();
        *arr = new_arr;
        *cap = new_cap;
    }
}

/* Mark an odd number as composite in the bitset */
static inline void set_bit(uint8_t *bits, size_t idx)
{
    bits[idx >> 3] |= (uint8_t)(1U << (idx & 7));
}

/* Incrementally extend the sieve and prime lists up to new_limit. */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;
    idx_t old_limit = sieve_limit;

    /* Size of bitset for odds: numbers 3,5,7,…,new_limit */
    size_t new_odd_count = (new_limit < 3) ? 0 : ((new_limit - 1) >> 1) + 1;
    size_t new_bytes = (new_odd_count + 7) >> 3;

    uint8_t *new_bits = (uint8_t *)realloc(sieve, new_bytes);
    if (!new_bits) abort();
    size_t old_odd_count = (old_limit < 3) ? 0 : ((old_limit - 1) >> 1) + 1;
    size_t old_bytes = (old_odd_count + 7) >> 3;
    if (new_bytes > old_bytes) memset(new_bits + old_bytes, 0, new_bytes - old_bytes);
    sieve = new_bits;

    /* Update composites for primes already known (up to sqrt(new_limit)) */
    idx_t sqrt_limit = 0;
    while ((sqrt_limit + 1) * (sqrt_limit + 1) <= new_limit) sqrt_limit++;

    for (size_t k = 1; k < primes_all_len && primes_all[k] <= (uint32_t)sqrt_limit; ++k) {
        uint32_t p = primes_all[k];
        idx_t start = (idx_t)p * (idx_t)p;
        idx_t min_start = old_limit + 1;
        if (start < min_start) {
            idx_t rem = min_start % p;
            idx_t tmp = (rem == 0) ? min_start : (min_start + (p - rem));
            if (tmp < start) tmp = start;
            start = tmp;
        }
        if (start % 2 == 0) start += p;          /* make odd */

        for (idx_t j = start; j <= new_limit; j += (p << 1)) {
            size_t idx = ((size_t)j - 3) >> 1;
            set_bit(sieve, idx);
        }
    }

    /* Scan new numbers and record primes */
    for (idx_t num = old_limit + 1; num <= new_limit; ++num) {
        if (num < 2) continue;
        if (num == 2) {
            if (primes_all_len == 0) { /* first prime to be inserted */
                ensure_cap(&primes_all_len, &primes_all_cap, &primes_all);
                primes_all[primes_all_len++] = 2;
                ensure_cap(&special_len, &special_cap, &special_primes);
                special_primes[special_len++] = 2;
            }
            continue;
        }
        if ((num & 1) == 0) continue;          /* even > 2 is composite */

        size_t idx = ((size_t)num - 3) >> 1;
        if (!is_bit(sieve, idx)) {
            ensure_cap(&primes_all_len, &primes_all_cap, &primes_all);
            primes_all[primes_all_len++] = (uint32_t)num;

            if ((num & 3) == 1) {              /* p ≡ 1 (mod 4) */
                ensure_cap(&special_len, &special_cap, &special_primes);
                special_primes[special_len++] = (uint32_t)num;
            }
        }
    }

    sieve_limit = new_limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* limit = n² (Euclidean norm squared) */
    idx_t limit = n * n;

    extend_sieve(limit);

    /* Binary search in special_primes for count ≤ limit */
    size_t lo = 0, hi = special_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((seq_t)special_primes[mid] <= limit)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}
