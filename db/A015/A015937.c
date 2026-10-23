/*
 * A015937  Positive integers n such that 2^n == 2^12 (mod n).
 * https://oeis.org/A015937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive integers n such that 2^n ≡ 2^12 (mod n). */
/* The function generates terms on demand by testing the congruence.
   Results are cached for repeated calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static inline uint64_t mod_pow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1U)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* Resize a dynamic array of seq_t to hold at least 'need' elements. */
static void ensure_capacity(size_t need, seq_t **buf, size_t *cap)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(*buf, new_cap * sizeof(seq_t));
    if (!new_buf) abort();               /* allocation failure */
    *buf = new_buf;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for index 0 */

    /* Static state for caching generated terms */
    static seq_t *seq   = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static uint64_t last_checked = 0;

    /* Generate terms up to index n */
    while (seq_len < n)
    {
        uint64_t cand = last_checked + 1;
        last_checked = cand;

        uint64_t rem_exp   = mod_pow_uint64(2, cand, cand);
        uint64_t rem_target = 4096 % cand;

        if (rem_exp == rem_target)
        {
            /* Store the candidate */
            ensure_capacity(seq_len + 1, &seq, &seq_cap);
            seq[seq_len++] = (seq_t)cand;
        }
    }

    return seq[n - 1];
}
