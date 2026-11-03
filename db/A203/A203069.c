/*
 * A203069  Lexicographically earliest sequence of distinct positive numbers
 *          such that a(n-1)+a(n) is odd and composite.
 * https://oeis.org/A203069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9823 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203069: lexicographically earliest permutation of positive integers
   such that a(n‑1)+a(n) is odd and composite.  The implementation
   lazily computes terms, storing them in a static array and keeping a
   Boolean array of used numbers.  It works for any seq_t type (built‑in
   or arbitrary‑precision) and accepts calls in arbitrary order.  The next
   term is always the smallest unused integer whose parity is opposite
   from the previous term and whose sum with the previous term is
   composite. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t *seq       = NULL;   /* the sequence values          */
static size_t seq_cap   = 0;      /* allocated capacity of seq    */
static size_t seq_len   = 0;      /* number of terms stored       */

static bool *used       = NULL;   /* Boolean array of used integers */
static size_t used_cap  = 0;      /* allocated size of used array  */

/* Ensure that the used array can index up to 'idx' (inclusive). */
static void ensure_used_cap(size_t idx)
{
    if (idx < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap << 1U : 16;
    while (new_cap <= idx)
        new_cap <<= 1U;
    bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_used)
        abort();
    /* Newly allocated slots are initially false. */
    memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used      = new_used;
    used_cap  = new_cap;
}

/* Ensure that the sequence array can hold at least 'len' elements. */
static void ensure_seq_cap(size_t len)
{
    if (len <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1U : 4;
    while (new_cap < len)
        new_cap <<= 1U;
    seq_t *new_seq = mem_resize(seq_t, seq, new_cap);
    if (!new_seq)
        abort();
    seq      = new_seq;
    seq_cap  = new_cap;
}

/* Mark the integer value 'v' as used. */
static void mark_used(seq_t v)
{
    size_t idx = (size_t)v;          /* seq_t values are non‑negative */
    ensure_used_cap(idx);
    used[idx] = true;
}

/* Generate sequence terms up until index n (1‑based). */
static void generate_up_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        if (seq_len == 0) {
            /* a(1) = 1 */
            ensure_seq_cap(1);
            seq[0] = 1;
            mark_used(1);
            seq_len = 1;
            continue;
        }

        seq_t prev = seq[seq_len - 1];
        bool need_odd = (prev & 1) == 0;          /* parity opposite to prev */
        size_t cand = need_odd ? (size_t)1 : (size_t)2;

        while (true) {
            ensure_used_cap(cand);
            if (!used[cand]) {
                seq_t sum = prev + (seq_t)cand;
                if (!isPrimeGeneric(sum)) {
                    /* Found the candidate. */
                    ensure_seq_cap(seq_len + 1);
                    seq[seq_len++] = (seq_t)cand;
                    mark_used(cand);
                    break;
                }
            }
            cand += 2; /* next candidate of same parity */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */
    generate_up_to(n);
    return seq[(size_t)(n - 1)];
}
