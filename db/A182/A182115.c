/*
 * A182115  Lexicographically earliest permutation of the positive integers
 *          such that a(n+a(n)+1) is prime if and only if a(n) is prime.
 * https://oeis.org/A182115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: lexicographically earliest permutation of the
 * positive integers such that a(n+a(n)+1) is prime iff a(n) is prime.
 * Sequence is 1‑based (a(1) = 1).  The function is memoised and can
 * be called in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *seq = NULL;           /* sequence values, 1‑based   */
static size_t seq_cap = 0;          /* capacity of seq array      */
static idx_t seq_len = 0;           /* largest assigned index     */

static uint8_t *req = NULL;         /* pending prime requirement
                                      1 = prime, 2 = composite   */
static size_t req_cap = 0;          /* capacity of req array       */

static uint8_t *used_flag = NULL;   /* used_flag[v] = 1 if value v already used  */
static size_t used_cap = 0;          /* capacity of used_flag array   */
static idx_t next_unused = 1;       /* next smallest unused number    */

/* ------------------------------------------------------------------ */
/* Helpers (cast void* results of realloc for C++ compatibility)    */

#ifdef __cplusplus
#  define REINTERP(ptr, type) static_cast<type>(ptr)
#else
#  define REINTERP(ptr, type) ((type)(ptr))
#endif

/* ------------------------------------------------------------------ */
static void ensure_seq_cap(size_t need)
{
    if (need > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 2;
        if (new_cap < need) new_cap = need;
        seq = seq_realloc(seq, seq_cap, new_cap);
        seq_cap = new_cap;
    }
}

static void ensure_req_cap(size_t need)
{
    if (need > req_cap) {
        size_t new_cap = req_cap ? req_cap * 2 : 2;
        if (new_cap < need) new_cap = need;
        uint8_t *tmp = REINTERP(realloc(req, new_cap), uint8_t*);
        if (!tmp) abort();
        memset(tmp + req_cap, 0, new_cap - req_cap);
        req = tmp;
        req_cap = new_cap;
    }
}

static void ensure_used_cap(idx_t val)
{
    size_t v = (size_t)val;
    if (v >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 2;
        if (new_cap <= v) new_cap = v + 1;
        uint8_t *tmp = REINTERP(realloc(used_flag, new_cap), uint8_t*);
        if (!tmp) abort();
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used_flag = tmp;
        used_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
static void generate_to_n(idx_t n)
{
    while (seq_len < n) {
        idx_t pos = seq_len + 1;          /* position to fill next       */
        ensure_seq_cap(pos + 1);
        ensure_req_cap(pos + 1);

        uint8_t req_pos = 0;
        if (pos < req_cap) req_pos = req[(size_t)pos];

        idx_t cand = next_unused;

        for (;;) {
            ensure_used_cap(cand);
            if (used_flag[(size_t)cand]) { ++cand; continue; }

            bool prime = is_prime_big((seq_t)cand);

            if (req_pos && ((req_pos == 1) != prime)) {
                ++cand; continue;
            }

            idx_t t = pos + (idx_t)cand + 1ULL;

            bool conflict = false;
            if (t <= seq_len) {
                bool pt = is_prime_big(seq[t]);
                if (pt != prime) conflict = true;
            } else {
                ensure_req_cap(t + 1);
                uint8_t req_t = req[(size_t)t];
                if (req_t && ((req_t == 1) != prime)) conflict = true;
            }

            if (conflict) { ++cand; continue; }

            /* Accept candidate */
            used_flag[(size_t)cand] = 1;
            seq[pos] = (seq_t)cand;
            seq_len = pos;

            if (cand == next_unused) {
                do {
                    ++next_unused;
                    if ((size_t)next_unused >= used_cap) ensure_used_cap(next_unused);
                } while (used_flag[(size_t)next_unused]);
            }

            if (t > seq_len) {
                ensure_req_cap(t + 1);
                req[(size_t)t] = prime ? 1 : 2;
            }

            break;    /* move to next position */
        }
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    generate_to_n(n);
    return seq[n];
}
