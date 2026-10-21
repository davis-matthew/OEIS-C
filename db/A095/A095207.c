/*
 * A095207  Lexicographically first sequence in which every positive integer k
 *          appears tau(k)=A000005(k) times and no two consecutive terms are
 *          equal.
 * https://oeis.org/A095207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc, tau_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically first sequence where each integer k appears
   tau(k) times (tau = number of divisors of k) and no consecutive
   terms are equal.  The algorithm keeps a remaining‑use counter
   for every integer and, for each position, picks the smallest
   integer that still has uses left and differs from the previous
   term.  The sequence is built up to the requested index n and
   cached for future calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/tau_of.h"

/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/* Static storage for the running sequence */
static seq_t *seq    = NULL;        /* seq[0] = a(1), seq[1] = a(2), ...   */
static size_t seq_cap = 0;          /* allocated capacity */
static idx_t  seq_len = 0;          /* number of elements built */

/* ----------------------------------------------------------------- */
/* Static storage for τ(k) and remaining uses rem(k)  (indices start at 1) */
static uint64_t *tau = NULL;   /* tau[k] = τ(k) */
static uint64_t *rem = NULL;   /* rem[k] = remaining uses of k */
static size_t tau_cap = 0;      /* allocated capacity of tau/rem */

/* ----------------------------------------------------------------- */
/* Ensure that the seq array can hold at least 'need' elements */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;

    if (seq == NULL) {
        seq = seq_alloc(new_cap);
    } else {
        seq = seq_realloc(seq, seq_cap, new_cap);
    }
    seq_cap = new_cap;
}

/* ----------------------------------------------------------------- */
/* Ensure that tau/rem arrays are valid for indices up to 'new_cap' */
static void ensure_tau_cap(size_t new_cap)
{
    if (tau_cap >= new_cap) return;
    size_t old_cap = tau_cap;
    size_t cap = tau_cap ? tau_cap << 1 : 8;
    while (cap < new_cap) cap <<= 1;

    uint64_t *new_tau = (uint64_t *)realloc(tau, (cap + 1) * sizeof(uint64_t));
    uint64_t *new_rem = (uint64_t *)realloc(rem, (cap + 1) * sizeof(uint64_t));
    if (!new_tau || !new_rem) return;     /* allocation failure ignored – caller will retry */

    tau = new_tau;
    rem = new_rem;

    /* initialise new entries */
    for (size_t k = old_cap + 1; k <= cap; ++k) {
        tau[k] = tau_of(k);
        rem[k] = tau[k];
    }
    tau_cap = cap;
}

/* ----------------------------------------------------------------- */
/* Initialization (called once) */
static void seq_init(void)
{
    if (seq_len != 0 || seq_cap != 0) return;   /* already initialised */
    ensure_seq_cap(16);
    ensure_tau_cap(64);
}

/* ----------------------------------------------------------------- */
/* Required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but safe */
    seq_init();

    /* Build sequence up to term n */
    while (seq_len < n) {
        seq_t prev = (seq_len > 0) ? seq[seq_len - 1] : 0;
        size_t k = 1;
        /* Find the smallest k with remaining uses and different from prev */
        for (;;) {
            if (k > tau_cap) ensure_tau_cap(k);
            if (rem[k] > 0 && ((seq_len == 0) || ((seq_t)k != prev))) break;
            ++k;
        }
        /* Append chosen value */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = (seq_t)k;
        rem[k]--;               /* consume one use */
    }

    return seq[n - 1];
}
