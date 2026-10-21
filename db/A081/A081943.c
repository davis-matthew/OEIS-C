/*
 * A081943  a(1) = 1, a(n)= smallest number not occurring earlier such that
 *          a(n-1)*a(n) -1 is a prime. re-arrangement of natural numbers such
 *          that the product of adjacent terms is one more than a prime.
 * https://oeis.org/A081943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A081943: a(1)=1; a(n) = smallest unused integer such that a(n‑1)*a(n)-1 is prime.  */
/*  The function builds a cache of all terms and a dynamic bitset of used numbers,
 *  so that each call runs in O(max(n, *largest_term*)).
 *  It uses a fast deterministic Miller–Rabin test for 64‑bit integers.
 */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"

static seq_t *seq      = NULL;       /* 0‑based array of computed terms, seq[0] = a(1) */
static size_t seq_cap  = 0;          /* allocated size of seq[] */
static idx_t  seq_len  = 0;          /* number of terms stored (>=0) */

static uint8_t *used_bits = NULL;    /* bitset of numbers already used (1‑based) */
static size_t   used_bits_nbits = 0; /* number of bits currently allocated */

/*---------------------------------------------------------------*/
/*  Simple deterministic Miller‑Rabin for 64‑bit integers          */
/*---------------------------------------------------------------*/

static inline uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1 % mod;
    while (e) {
        if (e & 1) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1;
    }
    return r;
}
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    /* Miller–Rabin with deterministic bases for 64‑bit */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}
static bool is_prime_u128(__uint128_t n) {
    if (n <= (uint64_t)-1) return is_prime_u64((uint64_t)n);
    /* For larger values, fall back to a weak test (not needed for this sequence). */
    return false;
}

/*---------------------------------------------------------------*/
/*  Dynamic bitset helpers                                       */
/*---------------------------------------------------------------*/
static void ensure_used_bits(idx_t need) {
    idx_t bits_needed = need;           /* need covers numbers 1..need */
    if ((size_t)bits_needed <= used_bits_nbits) return;
    size_t new_nbits = used_bits_nbits ? used_bits_nbits * 2 : 64;
    while (new_nbits < (size_t)bits_needed) new_nbits <<= 1;
    size_t new_bytes = (new_nbits + 7) >> 3;
    uint8_t *tmp = (uint8_t *)realloc(used_bits, new_bytes);
    if (!tmp) exit(EXIT_FAILURE);
    /* zero the newly allocated part */
    if (used_bits) {
        for (size_t i = used_bits_nbits >> 3; i < new_bytes; ++i) tmp[i] = 0;
    } else {
        for (size_t i = 0; i < new_bytes; ++i) tmp[i] = 0;
    }
    used_bits = tmp;
    used_bits_nbits = new_nbits;
}
static inline void mark_used(idx_t n) {
    if (n == 0) return;
    size_t bit = (size_t)(n - 1);
    ensure_used_bits(n);
    used_bits[bit >> 3] |= (uint8_t)(1u << (bit & 7));
}
static inline bool is_used(idx_t n) {
    if (n == 0) return false;
    size_t bit = (size_t)(n - 1);
    if (bit >= used_bits_nbits) return false;
    return (used_bits[bit >> 3] & (uint8_t)(1u << (bit & 7))) != 0;
}

/*---------------------------------------------------------------*/
/*  Main sequence function                                       */
/*---------------------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* indices start at 1 */
    /* Initialise on first call */
    if (seq_len == 0) {
        size_t init_cap = 16;
        seq = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
        seq_cap = init_cap;
        seq[0] = (seq_t)1;                  /* a(1) = 1 */
        seq_len = 1;
        mark_used(1);
    }
    /* Grow sequence up to n */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        idx_t candidate = 1;
        for (;;) {
            if (is_used(candidate)) { candidate++; continue; }
            unsigned __int128 prod = (unsigned __int128)prev * candidate - 1;
            bool prime;
            if (prod <= (unsigned __int128)(UINT64_MAX))
                prime = is_prime_u64((uint64_t)prod);
            else
                prime = is_prime_u128(prod);
            if (prime) break;
            candidate++;
        }
        /* Append candidate */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[seq_len] = (seq_t)candidate;
        mark_used(candidate);
        ++seq_len;
    }
    return seq[n - 1];
}
