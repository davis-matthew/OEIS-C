/*
 * A095167  Triangle read by rows in which the n-th row contains n numbers
 *          noncoprime to n and not occurring in earlier rows.
 * https://oeis.org/A095167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 9958, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alg: generate rows of a triangle where the n-th row contains
 * n numbers that are not coprime to n and have not appeared in
 * earlier rows; output is the triangle read by rows.
 *
 * This implementation maintains a dynamic buffer of already
 * produced terms, a bitset of used numbers, and a small‑prime‑factor
 * array up to the current row.  Each row is formed by repeatedly
 * selecting the smallest unused multiple of any prime divisor of
 * the current row number.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* ----- static state ------------------------------------------------------ */

static seq_t  *seq_buf   = NULL;   /* sequence values               */
static size_t  seq_len   = 0;      /* current number of stored      */
static size_t  seq_cap   = 0;      /* allocated buffer capacity     */

static uint64_t *used_bits = NULL;  /* bitset of used numbers       */
static size_t   used_cap   = 0;      /* number of 64‑bit words       */

static uint32_t *spf = NULL;        /* smallest prime factor array   */
static size_t   spf_cap = 0;        /* current capacity of spf       */

/* ----- helper utilities -------------------------------------------- */

/* Ensure used_bits can address the number x (x ≥ 1). */
static void ensure_used_capacity(uint64_t x)
{
    size_t idx = x >> 6;
    if (idx >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 16;
        while (new_cap <= idx) new_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(used_bits, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(uint64_t));
        used_bits = tmp;
        used_cap = new_cap;
    }
}

/* Test whether number x has been used already. */
static bool is_used(uint64_t x)
{
    if (x == 0) return false;            /* avoid index 0 */
    ensure_used_capacity(x);
    return (used_bits[x >> 6] >> (x & 63)) & 1ULL;
}

/* Mark number x as used. */
static void set_used(uint64_t x)
{
    if (x == 0) return;
    ensure_used_capacity(x);
    used_bits[x >> 6] |= (1ULL << (x & 63));
}

/* Append a value to the sequence buffer, growing as needed. */
static void seq_push(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t*)realloc(seq_buf, seq_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_buf = tmp;
    }
    seq_buf[seq_len++] = val;
}

/* Resize the spf array to hold at least limit elements. */
static void ensure_spf(size_t limit)
{
    if (spf_cap > limit) return;
    size_t new_cap = spf_cap ? spf_cap << 1 : 64;
    while (new_cap <= limit) new_cap <<= 1;
    uint32_t *tmp = (uint32_t*)realloc(spf, new_cap * sizeof(uint32_t));
    if (!tmp) abort();

    /* initialise new region */
    for (size_t i = spf_cap; i < new_cap; ++i) tmp[i] = (uint32_t)i;

    /* sieve of Eratosthenes on new region */
    for (size_t i = 2; i * i < new_cap; ++i) {
        if (tmp[i] == (uint32_t)i) {
            for (size_t j = i * i; j < new_cap; j += i) {
                if (tmp[j] == (uint32_t)j) tmp[j] = (uint32_t)i;
            }
        }
    }
    spf = tmp;
    spf_cap = new_cap;
}

/* ----- core algorithm ----------------------------------------------- */

/* Generate the row numbered r and append its values to seq_buf. */
static void generate_row(idx_t r)
{
    if (r == 1) {                  /* special case: only number 1 */
        seq_push((seq_t)1);
        set_used(1);
        return;
    }

    /* factor r into distinct primes */
    ensure_spf((size_t)r);
    uint32_t primes[16];
    size_t pc = 0;
    uint64_t t = (uint64_t)r;
    while (t > 1) {
        uint32_t p = spf[(size_t)t];
        primes[pc++] = p;
        while (t % p == 0) t /= p;
    }

    /* current next unused multiple for each prime */
    uint64_t curr[16];
    for (size_t i = 0; i < pc; ++i) curr[i] = primes[i];

    /* collect r numbers */
    for (idx_t cnt = 0; cnt < r; ++cnt) {
        uint64_t min_val = UINT64_MAX;
        size_t   min_idx = 0;
        for (size_t i = 0; i < pc; ++i) {
            uint64_t cand = curr[i];
            while (is_used(cand)) cand += primes[i];
            curr[i] = cand;
            if (cand < min_val) { min_val = cand; min_idx = i; }
        }
        seq_push((seq_t)min_val);
        set_used(min_val);
        curr[min_idx] += primes[min_idx];
    }
}

/* Ensure the sequence has at least n elements, generating rows as needed. */
static void compute_up_to(idx_t n)
{
    static idx_t last_row = 0;      /* last row generated so far */
    while (seq_len < n) {
        ++last_row;
        generate_row(last_row);
    }
}

/* ----- public entry point ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n=0, but safe */
    compute_up_to(n);
    return seq_buf[n - 1];
}
