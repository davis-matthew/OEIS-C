/*
 * A231877  Numbers k such that omega(k)^2 (cf. A001221) does not divide k.
 * https://oeis.org/A231877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 1300, u32 n <= 1300, u64 n <= 1300,
 *            u128 n <= 1300, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th positive integer k such that ω(k)^2 does not divide k.
   ω(k) is the number of distinct prime factors of k (OEIS A001221).  The first
   few terms are 1, 6, 10, 14, 15, 18, … */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_floor_isqrt_u64.h"

static seq_t *seq = NULL;          /* stored sequence values */
static size_t seq_len = 0;         /* number of values stored */
static size_t seq_cap = 0;         /* capacity of seq array */
static uint64_t cur_k = 0;         /* next integer to test */

static uint64_t *primes = NULL;    /* array of primes for factorisation */
static size_t primes_len = 0;      /* number of stored primes */
static size_t primes_cap = 0;      /* capacity of primes array */

/* Grow the seq array when capacity is reached */
static void grow_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap << 1 : 32;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) return; /* avoid crash on OOM */
    seq = tmp;
    seq_cap = new_cap;
}

/* Grow the primes array when capacity is reached */
static void grow_primes(void)
{
    size_t new_cap = primes_cap ? primes_cap << 1 : 32;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) return;
    primes = tmp;
    primes_cap = new_cap;
}

/* Ensure primes list contains all primes ≤ limit */
static void ensure_primes_up_to(uint64_t limit)
{
    uint64_t start = primes_len ? primes[primes_len - 1] + 1 : 2;
    for (uint64_t p = start; p <= limit; ++p) {
        bool is_p = true;
        uint64_t r = _floor_isqrt_u64(p);
        for (size_t i = 0; i < primes_len && primes[i] <= r; ++i) {
            if (p % primes[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (primes_len >= primes_cap) grow_primes();
            primes[primes_len++] = p;
        }
    }
}

/* Number of distinct prime factors of n (ω(n)) */
static unsigned omega_of(uint64_t n)
{
    if (n <= 1) return 0;              /* ω(1) = 0 by convention */
    unsigned cnt = 0;
    uint64_t temp = n;
    uint64_t limit = _floor_isqrt_u64(temp);
    ensure_primes_up_to(limit);
    for (size_t i = 0; i < primes_len && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            ++cnt;
            while (temp % p == 0) temp /= p;
            limit = _floor_isqrt_u64(temp);
        }
    }
    if (temp > 1) ++cnt;                /* remaining prime factor */
    return cnt;
}

/* Append a value to the sequence array */
static void append_seq(seq_t val)
{
    if (seq_len >= seq_cap) grow_seq();
    seq[seq_len++] = val;
}

/* Public function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n<1, return 0 */
    while (seq_len < (size_t)n) {
        ++cur_k;
        unsigned w = omega_of(cur_k);
        if (cur_k == 1 || ((uint64_t)w * (uint64_t)w != 0 && cur_k % ((uint64_t)w * (uint64_t)w) != 0)) {
            append_seq((seq_t)cur_k);
        }
    }
    return seq[(size_t)n - 1];
}
