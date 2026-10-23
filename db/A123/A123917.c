/*
 * A123917  a(1)=1. a(n) = (the highest exponent in the prime-factorization of
 *          n)th integer from among those positive integers not occurring
 *          earlier in the sequence.
 * https://oeis.org/A123917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   highest_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A123917:
   a(1) = 1.
   For n>1 let e(n) be the largest exponent in the prime factorization of n.
   a(n) is the e(n)-th positive integer not already appearing among a(1)…a(n‑1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/highest_exponent.h"

/* ---------- static state ---------- */
static seq_t *sequence = NULL;      /* array of computed terms            */
static size_t seq_cap = 0;          /* capacity of 'sequence'             */
static idx_t  seq_len  = 0;         /* number of terms stored             */

static unsigned char *used = NULL;  /* 0/1 flags for numbers already used */
static size_t used_cap = 0;         /* capacity of 'used' array           */

/* Ensure 'used' array can index up to val (inclusive). */
static void ensure_used_cap(idx_t val)
{
    if ((size_t)val < used_cap) return;
    size_t new_cap = used_cap ? used_cap : 1;
    while (new_cap <= (size_t)val) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp) abort();
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* Ensure sequence array can hold at least n terms. */
static void ensure_seq_cap(idx_t n)
{
    if (seq_cap >= (size_t)n) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    sequence = tmp;
    seq_cap = new_cap;
}

/* Compute the sequence up to index n (inclusive). */
static void compute_until(idx_t n)
{
    if (n <= seq_len) return;          /* already computed */

    ensure_seq_cap(n);

    for (idx_t k = seq_len + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1) {
            val = 1;
        } else {
            unsigned int lam = highest_exponent(k);
            /* Find the lam-th unused positive integer. */
            int count = 0;
            idx_t cand = 1;
            while (1) {
                ensure_used_cap(cand);
                if (!used[(size_t)cand]) {
                    ++count;
                    if ((unsigned int)count == lam) {
                        val = (seq_t)cand;
                        break;
                    }
                }
                ++cand;
            }
        }
        sequence[k - 1] = val;
        /* Mark this value as used. */
        if (val >= (seq_t)used_cap) ensure_used_cap((idx_t)val);
        used[(size_t)val] = 1;
    }
    seq_len = n;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) abort();             /* undefined for n < 1 */
    compute_until(n);
    return sequence[n - 1];
}
