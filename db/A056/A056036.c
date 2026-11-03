/*
 * A056036  5-morphic but not bimorphic, automorphic nor trimorphic.
 * https://oeis.org/A056036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   ipow_i32, mem, num_digits0, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056036: 5‑morphic numbers that are not bimorphic, automorphic or trimorphic.
   A number a with d decimal digits satisfies a^5 ≡ a (mod 10^d) but not
   a^2 ≡ a (mod 10^d) nor a^3 ≡ a (mod 10^d).  The function returns the
   n‑th such number (n ≥ 1).  Implements a simple search with checks
   performed by the utility functions mod_pow, pow_seq, and digit_count. */

#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits0.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

static seq_t *seq = NULL;      /* cached list of sequence values */
static size_t seq_size = 0;    /* number of cached values */
static size_t seq_cap  = 0;    /* allocated capacity of seq */
static idx_t next_checked = 1; /* next integer candidate to test */

/* Allocate or enlarge the cache to hold at least “need” elements. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = mem_resize(seq_t, seq, new_cap);
    seq_cap = new_cap;
}

/* Return the n‑th member of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Build the cache until it contains at least n terms. */
    while (seq_size < (size_t)n) {
        /* Number of decimal digits of the candidate (next_checked). */
        unsigned long long d = num_digits0((uint64_t)next_checked);

        /* Modulus 10^d for the congruence checks. */
        seq_t mod = ipow_i32((seq_t)10, (int)d);

        seq_t cand = (seq_t)next_checked;

        /* 5‑morphic test */
        if (powmod(cand, (idx_t)5, mod) == cand) {
            /* Exclude automorphic (2‑morphic) and trimorphic (3‑morphic). */
            if (powmod(cand, (idx_t)2, mod) != cand &&
                powmod(cand, (idx_t)3, mod) != cand) {
                ensure_seq_capacity(seq_size + 1);
                seq[seq_size++] = cand;
            }
        }
        ++next_checked;
    }
    return seq[n - 1];
}
