/*
 * A022935  a(n) = a(n-1) + c(n-1) for n >= 2, a( ) increasing, given a(1)=3,
 *          where c( ) is complement of a( ).
 * https://oeis.org/A022935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 347, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; a(n)=a(n-1)+c(n-1) for n>=2, where c(k) is the
   k-th positive integer that does not appear among a(1),…,a(k). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  Bit‑set helpers for used numbers  ---------- */
static uint64_t *used_bits = NULL;   /* 1 bit = number already used; index 1 maps to bit 0 */
static size_t used_words   = 0;      /* number of uint64_t words allocated */

static inline void ensure_used_words(size_t w) {
    if (w >= used_words) {
        size_t new_words = used_words ? used_words * 2 : 128;
        while (new_words <= w) new_words <<= 1;            /* grow until big enough */
        uint64_t *tmp = (uint64_t *)realloc(used_bits,
                                          new_words * sizeof(uint64_t));
        if (!tmp) abort();
        for (size_t i = used_words; i < new_words; ++i) tmp[i] = 0;
        used_bits = tmp;
        used_words = new_words;
    }
}

static inline bool is_used(idx_t v) {           /* v > 0 */
    idx_t idx = v - 1;                          /* 0‑based bit index */
    size_t w = idx >> 6;
    size_t b = idx & 63;
    if (w >= used_words) return false;
    return (used_bits[w] >> b) & 1ULL;
}

static inline void set_used(idx_t v) {          /* mark number as used */
    idx_t idx = v - 1;
    size_t w = idx >> 6;
    size_t b = idx & 63;
    ensure_used_words(w);
    used_bits[w] |= 1ULL << b;
}

/* ----------  Sequence cache helpers  ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;        /* allocated capacity */
static idx_t seq_size   = 0;        /* number of terms already computed */

/* Reallocation for seq_cache – works in both C and C++ */
static void ensure_cache_capacity(size_t n) {
    if (seq_size >= n) return;

    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap < n) new_cap <<= 1;

    seq_t *new_buf;
#ifdef __cplusplus
    new_buf = new seq_t[new_cap];
    for (size_t i = 0; i < seq_cap; ++i) new_buf[i] = seq_cache[i];
    delete[] seq_cache;
#else
    new_buf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
#endif
    if (!new_buf) abort();
    seq_cache = new_buf;
    seq_cap   = new_cap;
}

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n=0 – safe return */

    /* Grow the cache to hold terms up to n */
    ensure_cache_capacity(n);

    /* First term initialization */
    if (seq_size == 0) {
        seq_cache[0] = (seq_t)3;
        seq_size = 1;
        set_used((idx_t)3);
    }

    /* Static state for scanning missing numbers */
    static idx_t next_val     = 1;          /* next integer to test for missing */
    static idx_t missing_cnt  = 0;          /* how many missing numbers found */

    /* Compute terms from seq_size+1 up to n */
    for (idx_t k = seq_size + 1; k <= n; ++k) {
        idx_t target = k - 1;            /* need the (k-1)-th missing number */

        /* Scan forward until we have target missing numbers */
        while (missing_cnt < target) {
            size_t w = (next_val - 1) >> 6;
            ensure_used_words(w);
            if (!((used_bits[w] >> ((next_val - 1) & 63)) & 1ULL)) {
                ++missing_cnt;            /* we found a missing number */
                if (missing_cnt == target) break;
            }
            ++next_val;
        }

        /* next_val is now the desired missing number c(k-1) */
        seq_t c = (seq_t)next_val;

        /* New term = previous term + c */
        seq_t prev = seq_cache[seq_size - 1];
        seq_t new_val = prev + c;
        seq_cache[seq_size] = new_val;
        ++seq_size;

        /* Mark the new term as used and advance the scan */
        set_used((idx_t)new_val);
        ++next_val;
    }

    return seq_cache[n - 1];
}
