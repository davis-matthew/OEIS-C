/*
 * A082019  Diagonal of square array T(n,k) with T(1,1) = 1 where
 *          antidiagonals are filled alternating upwards and downwards with
 *          the smallest number not already used such that the n-th
 *          antidiagonal sum is a multiple of n.
 * https://oeis.org/A082019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A082019:
   a(n) is the main diagonal element of the infinite array T(n,k) obtained
   by filling each antidiagonal (index ≡ n) with the smallest unused
   natural numbers so that the sum of the antidiagonal is divisible by
   its index.  The filling direction alternates for successive antidiagonals. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Bitset for tracking used numbers (indexed by natural numbers ≥ 1). */
static uint64_t *used_bits = NULL;
static size_t used_bits_len = 0;      /* number of 64‑bit words */

/* Ensure the bitset can address index `idx`. */
static void ensure_used_capacity(uint64_t idx)
{
    size_t need_words = (idx >> 6) + 1;
    if (need_words <= used_bits_len)
        return;
    size_t new_len = used_bits_len ? used_bits_len * 2 : 16;
    while (new_len < need_words)
        new_len <<= 1;
    uint64_t *new_bits = (uint64_t *)realloc(used_bits, new_len * sizeof(uint64_t));
    if (!new_bits)
        abort();                /* memory exhausted */
    /* initialise the new part to zero */
    for (size_t i = used_bits_len; i < new_len; ++i)
        new_bits[i] = 0;
    used_bits = new_bits;
    used_bits_len = new_len;
}

/* Bit operations. */
static inline bool used_at(uint64_t n)
{
    ensure_used_capacity(n);
    return (used_bits[n >> 6] >> (n & 63)) & 1ULL;
}
static inline void set_used(uint64_t n)
{
    ensure_used_capacity(n);
    used_bits[n >> 6] |= 1ULL << (n & 63);
}
static inline void clear_used(uint64_t n)
{
    ensure_used_capacity(n);
    used_bits[n >> 6] &= ~(1ULL << (n & 63));
}

/* Temporary buffer for an antidiagonal. */
static seq_t *temp_buf = NULL;
static size_t temp_cap = 0;

/* Storage for diagonal answers: diag_ans[i] = a(i+1) */
static seq_t *diag_ans = NULL;
static size_t diag_cap = 0;

/* The next antidiagonal index to process.  Antidiagonals are numbered
   starting at 1 and have lengths 1,2,3,... respectively. */
static idx_t next_diagonal = 1;

/* Smallest unused natural number that has not yet been processed. */
static uint64_t next_candidate = 1;

/* Compute one antidiagonal of length `len` and, if `len` is odd,
   store its median element in diag_ans. */
static void compute_diagonal(idx_t len)
{
    size_t m = (size_t)len;
    if (m > temp_cap) {
        temp_cap = m;
        temp_buf = (seq_t *)realloc(temp_buf, temp_cap * sizeof(seq_t));
        if (!temp_buf)
            abort();
    }

    seq_t sum = 0;
    for (size_t i = 0; i < m; ++i) {
        /* Find the smallest unused number. */
        uint64_t cand = next_candidate;
        while (used_at(cand))
            ++cand;
        temp_buf[i] = (seq_t)cand;
        set_used(cand);
        next_candidate = cand + 1;
        sum += temp_buf[i];
    }

    /* Adjust the last element until the antidiagonal sum is divisible by its length. */
    seq_t rem = sum % (seq_t)len;
    while (rem != 0) {
        uint64_t old_val = (uint64_t)temp_buf[m - 1];
        clear_used(old_val);
        uint64_t new_val = old_val + 1;
        while (used_at(new_val))
            ++new_val;
        temp_buf[m - 1] = (seq_t)new_val;
        set_used(new_val);
        sum += (seq_t)(new_val - old_val);
        rem = sum % (seq_t)len;
        /* Ensure the candidate pointer stays minimal. */
        if (old_val < next_candidate)
            next_candidate = old_val;
    }

    /* If len is odd, it corresponds to a main diagonal element. */
    if (len & 1ULL) {
        idx_t d = len;                                 /* d = 2*n - 1 */
        idx_t diag_idx = (d - 1) / 2 + 1;               /* n = (d+1)/2 */
        if (diag_idx > diag_cap) {
            size_t new_cap = diag_cap ? diag_cap * 2 : 8;
            while (new_cap < (size_t)diag_idx)
                new_cap <<= 1;
            diag_ans = (seq_t *)realloc(diag_ans, new_cap * sizeof(seq_t));
            if (!diag_ans)
                abort();
            diag_cap = new_cap;
        }
        size_t median = (m - 1) / 2;
        diag_ans[diag_idx - 1] = temp_buf[median];
    }
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset 1; behaviour for n==0 is undefined */

    /* The main diagonal element at (n,n) lies on antidiagonal index 2*n-1. */
    idx_t target_d = 2 * n - 1;
    while (next_diagonal <= target_d) {
        compute_diagonal(next_diagonal);
        ++next_diagonal;
    }
    return diag_ans[n - 1];
}
