/*
 * A086305  Lexicographically earliest sequence of distinct numbers such that
 *          two successive terms have no common digit and have distinct prime
 *          signature.
 * https://oeis.org/A086305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086305: Lexicographically earliest sequence of distinct numbers
   such that consecutive terms have no common digit and distinct
   prime signature.  This implementation uses a greedy algorithm:
   at each step pick the smallest unused integer that satisfies the
   digit‐mask and prime‑signature constraints with the previous term. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/*   Dynamic array for the sequence (1‑indexed)                */
/*------------------------------------------------------------*/
static seq_t   *seq      = NULL;   /* array of terms               */
static size_t   seq_len  = 0;      /* number of terms stored       */
static size_t   seq_cap  = 0;      /* capacity of seq array        */

/*------------------------------------------------------------*/
/*   Sieve with smallest prime factor (spf)                    */
/*------------------------------------------------------------*/
static uint32_t *spf      = NULL;   /* spf[i] = smallest prime dividing i   */
static size_t   spf_cap   = 0;       /* capacity of spf array                */

static void init_spf(size_t limit)
{
    if (limit <= spf_cap) return;          /* already large enough */

    size_t new_cap = limit + 1;
    uint32_t *new_spf = (uint32_t *)realloc(spf, new_cap * sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);
    spf     = new_spf;
    spf_cap = new_cap;

    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    if (limit < 2) return;
    for (size_t i = 2; i * i <= limit; ++i)
        if (spf[i] == i)                     /* i is prime */
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = (uint32_t)i;
}

/*------------------------------------------------------------*/
/* Helper: convert a number to a digit mask (10 bits)        */
/*------------------------------------------------------------*/
static uint16_t digit_mask(seq_t n)
{
    uint16_t m = 0;
    if (n == 0) return 1U << 0;  /* digit 0 */
    while (n > 0) {
        uint32_t d = (uint32_t)(n % 10);
        m |= (1u << d);
        n /= 10;
    }
    return m;
}

/*------------------------------------------------------------*/
/* Helper: compute prime signature of a 64‑bit integer.  The   */
/* signature is an array of exponents sorted ascending.  This  */
/* will only be used for numbers that fit in 64 bits.         */
/*------------------------------------------------------------*/
static void prime_sig(uint64_t v, uint8_t *out, size_t *len_out)
{
    if (v <= 1) { *len_out = 0; return; }

    size_t len = 0;
    while (v > 1) {
        uint32_t p = spf[(size_t)v];
        uint8_t cnt = 0;
        while (v % p == 0) {
            v /= p;
            ++cnt;
        }
        out[len++] = cnt;
    }
    /* simple insertion sort (len <= 64) */
    for (size_t i = 1; i < len; ++i) {
        uint8_t key = out[i];
        size_t j = i;
        while (j > 0 && out[j-1] > key) {
            out[j] = out[j-1];
            --j;
        }
        out[j] = key;
    }
    *len_out = len;
}

/*------------------------------------------------------------*/
/* Check whether `candidate` can follow `prev`                */
/*------------------------------------------------------------*/
static bool candidate_ok(seq_t candidate, seq_t prev)
{
    /* digit mask constraint */
    uint16_t c_mask = digit_mask(candidate);
    uint16_t p_mask = digit_mask(prev);
    if (c_mask & p_mask) return false;

    /* prime signature constraint */
    uint32_t cand_u64 = (uint32_t)candidate;   /* safe for all tests */
    uint64_t cand64  = (uint64_t)cand_u64;
    uint32_t prev_u64 = (uint32_t)prev;
    uint64_t prev64    = (uint64_t)prev_u64;

    uint8_t c_sig[64], p_sig[64];
    size_t c_len, p_len;

    /* ensure spf large enough */
    if (cand64 >= spf_cap) init_spf((size_t)(cand64 * 2 + 10));
    if (prev64 >= spf_cap) init_spf((size_t)(prev64 * 2 + 10));

    prime_sig(cand64, c_sig, &c_len);
    prime_sig(prev64, p_sig, &p_len);
    if (c_len != p_len) return true;
    for (size_t i = 0; i < c_len; ++i)
        if (c_sig[i] != p_sig[i]) return true;
    return false;   /* signatures equal */
}

/*------------------------------------------------------------*/
/* Add term to sequence, extending storage if needed           */
/*------------------------------------------------------------*/
static void add_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 64 : seq_cap * 2;
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_seq) exit(EXIT_FAILURE);
        seq = new_seq; seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/*------------------------------------------------------------*/
/* Check if a value is already used in the sequence           */
/*------------------------------------------------------------*/
static bool used(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == val) return true;
    return false;
}

/*------------------------------------------------------------*/
/* Main function required by the problem                    */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; callers should never request 0 */

    /* ensure the sieve is initialized */
    if (!spf) init_spf(2000000);

    /* seed the sequence if empty */
    if (seq_len == 0) add_term((seq_t)1);   /* a(1)=1 */

    /* generate terms until we reach n */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        seq_t cand = 2;
        for (;;) {
            if (cand == prev || used(cand)) {
                ++cand;
                continue;
            }
            if (candidate_ok(cand, prev)) break;
            ++cand;
        }
        add_term(cand);
    }
    return seq[n - 1];
}
