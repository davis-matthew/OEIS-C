/*
 * A175349  a(n) is the smallest positive integer that, when written in
 *          binary, contains the binary representations of both the n-th prime
 *          and the n-th composite as (possibly overlapping) substrings.
 * https://oeis.org/A175349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the smallest integer whose binary representation contains
   the binary representations of the n-th prime and the n-th composite
   as (possibly overlapping) substrings.  The implementation enumerates
   all possible relative shifts between the two bit patterns, forms
   each feasible superstring without conflicts, and keeps the one with
   the smallest numeric value (and, equivalently, the shortest bit
   length).  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for primes and composites */
static uint64_t *primes = NULL;
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;
static uint64_t last_prime_checked = 1;

static uint64_t *composites = NULL;
static size_t   comp_cnt = 0;
static size_t   comp_cap = 0;
static uint64_t last_comp_checked = 3;

/* Grow an array of uint64_t to at least 'need' elements.  Requires a cast
   because realloc returns void *. */
static void grow_uint64_array(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (*arr) *cap = new_cap;
}

/* Test whether x >= 2 is prime using primes already found up to sqrt(x). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure at least n primes are stored. */
static void ensure_n_primes(idx_t n)
{
    while (prime_cnt < (size_t)n) {
        uint64_t cand = ++last_prime_checked;
        if (is_prime_uint64(cand)) {
            grow_uint64_array(&primes, &prime_cap, prime_cnt + 1);
            primes[prime_cnt++] = cand;
        }
    }
}

/* Ensure at least n composites are stored. */
static void ensure_n_composites(idx_t n)
{
    while (comp_cnt < (size_t)n) {
        uint64_t cand = ++last_comp_checked;
        if (!is_prime_uint64(cand)) {
            grow_uint64_array(&composites, &comp_cap, comp_cnt + 1);
            composites[comp_cnt++] = cand;
        }
    }
}

/* Return the number of bits required to represent x (x > 0). */
static inline int bitlen_seq_t(seq_t x)
{
    int len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return (len > 0) ? len : 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    /* Prepare the n-th prime and n-th composite */
    ensure_n_primes(n);
    ensure_n_composites(n);

    uint64_t prime_n   = primes[(size_t)(n - 1)];
    uint64_t comp_n    = composites[(size_t)(n - 1)];

    int lenP = 0;  /* bit length of prime */
    uint64_t tp = prime_n;
    while (tp) { ++lenP; tp >>= 1; }
    if (lenP == 0) lenP = 1;

    int lenC = 0;  /* bit length of composite */
    uint64_t tc = comp_n;
    while (tc) { ++lenC; tc >>= 1; }
    if (lenC == 0) lenC = 1;

    /* Store bits of prime and composite */
    unsigned char bitsP[64];
    unsigned char bitsC[64];
    for (int i = 0; i < lenP; ++i)
        bitsP[i] = (prime_n >> (lenP - 1 - i)) & 1U;
    for (int i = 0; i < lenC; ++i)
        bitsC[i] = (comp_n  >> (lenC - 1 - i)) & 1U;

    /* Enumerate all relative shifts */
    int min_shift = -(lenC - 1);
    int max_shift =  lenP - 1;

    seq_t best_val = 0;
    int best_len = 0x7fffffff;
    bool best_set = false;

    for (int s = min_shift; s <= max_shift; ++s) {
        /* Total length and array offset */
        int start_offset = (s < 0) ? -s : 0;
        int L = (s < 0) ?
                 (int) ( (s + lenC > lenP ? s + lenC : lenP) - s ) :
                 (int) ( (s + lenC > lenP ? s + lenC : lenP) );

        if (L <= 0 || L >= 128) continue;  /* safety, should never happen */

        int comb_bits[128];
        for (int i = 0; i < L; ++i) comb_bits[i] = -1;

        /* Overlay prime bits */
        for (int i = 0; i < lenP; ++i) {
            comb_bits[start_offset + i] = bitsP[i];
        }

        /* Overlay composite bits */
        bool conflict = false;
        for (int j = 0; j < lenC; ++j) {
            int pos = start_offset + s + j;
            if (pos < 0 || pos >= L) { conflict = true; break; }
            int val = bitsC[j];
            if (comb_bits[pos] == -1) {
                comb_bits[pos] = val;
            } else if (comb_bits[pos] != val) {
                conflict = true; break;
            }
        }
        if (conflict) continue;

        /* Leading bit must be 1 (always satisfied by construction) */
        if (comb_bits[0] != 1) continue;

        /* Compute integer value from combined bits */
        seq_t val = 0;
        for (int i = 0; i < L; ++i)
            val = (val << 1) | (seq_t)comb_bits[i];

        /* Update best candidate */
        if (!best_set ||
            L < best_len ||
            (L == best_len && val < best_val)) {
            best_set = true;
            best_len = L;
            best_val = val;
        }
    }

    return best_val;
}
