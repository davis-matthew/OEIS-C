/*
 * A091850  a(1) = 1; a(n) is the smallest positive integer not already in the
 *          sequence such that Sum_{k=1..n} k*a(k) is a prime.
 * https://oeis.org/A091850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 57, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime_big
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
#include "../utils/is_prime_big.h"

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
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
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
