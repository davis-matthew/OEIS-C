/*
 * A091850  a(1) = 1; a(n) is the smallest positive integer not already in the
 *          sequence such that Sum_{k=1..n} k*a(k) is a prime.
 * https://oeis.org/A091850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091850
   a(1) = 1; for n>1, a(n) is the smallest positive integer not already in
   the sequence such that  Σ_{k=1..n} k*a(k) is prime.
   The algorithm keeps the running sum and a list of previously used terms;
   for each new index it scans candidates in increasing order, checks
   membership by a linear search, and tests primality with the arbitrary‑precision
   helper is_prime_big().  This works for all provided builds. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"
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

/* ---------- dynamic array for the sequence ---------- */
static seq_t   *seq      = NULL;   /* values a(1) .. a(n)  */
static idx_t    seq_len  = 0;      /* current number of stored terms   */
static size_t   seq_cap  = 0;      /* allocated capacity              */
static seq_t    sum      = 0;      /* Σ_{k=1..seq_len} k*a(k)          */

static void ensure_seq_capacity(idx_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq = mem_resize(seq_t, seq, new_cap);
    seq_cap = new_cap;
}

/* ---------- prime test ---------- */
static inline bool is_prime(seq_t n) { return is_prime_big(n); }

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* initialise with the fixed first term */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        seq[0] = (seq_t)1;
        sum    = (seq_t)1 * (seq_t)1;   /* sum = 1*1 = 1 */
        seq_len = 1;
    }
    if (n <= seq_len) return seq[n-1];

    /* generate missing terms up to n */
    while (seq_len < n) {
        idx_t cur_index = seq_len + 1;   /* 1‑based index of the new term */
        seq_t cand = (seq_t)1;

        while (true) {
            /* check that cand is not already used */
            bool used = false;
            for (idx_t j = 0; j < seq_len; ++j) {
                if (seq[j] == cand) { used = true; break; }
            }
            if (used) { cand++; continue; }

            seq_t newSum = sum + (seq_t)cur_index * cand;
            if (is_prime(newSum)) {
                ensure_seq_capacity(seq_len + 1);
                seq[seq_len++] = cand;
                sum = newSum;
                break;                                 /* next term */
            }
            cand++;
        }
    }
    return seq[n-1];
}
