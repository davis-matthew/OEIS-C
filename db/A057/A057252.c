/*
 * A057252  Numbers n such that n | 10^n + 9^n + 8^n + 7^n + 6^n.
 * https://oeis.org/A057252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n such that n | 10^n + 9^n + 8^n + 7^n + 6^n
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   mulmod_seq, pow_mod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a brute‑force scan of the positive integers,
   testing the divisibility condition by modular exponentiation.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mulmod_seq.h"
#include "../utils/pow_mod_seq.h"

/* -------------- helper: modular multiplication ----------------- */

/* -------------- helper: fast modular exponentiation ------------- */

/* -------------- cache for the generated terms ------------------ */
static seq_t  *seq_cache = NULL;   /* cache for the terms of the sequence   */
static size_t  seq_cap   = 0;      /* allocated capacity                    */
static size_t  seq_len   = 0;      /* number of stored terms                 */
static idx_t   next_n    = 1;      /* next integer to test                   */

/* enlarge the cache so that it can hold at least ``needed`` terms   */
static void grow_cache(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* -------------------------------------------------------------- */
/* Return the n-th term of A057252 (1‑based).                     */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */

    /* ensure the cache can hold the result */
    if (seq_len < n)
        grow_cache(n);

    /* generate consecutive integers until we have n terms        */
    while (seq_len < n) {
        idx_t cand = next_n++;                  /* candidate integer */

        seq_t mod   = (seq_t)cand;             /* modulo is the candidate itself   */
        seq_t sum   = 0;                        /* running sum modulo cand          */

        sum += pow_mod_seq((seq_t)10, cand, mod);
        sum %= mod;
        sum += pow_mod_seq((seq_t)9,  cand, mod);
        sum %= mod;
        sum += pow_mod_seq((seq_t)8,  cand, mod);
        sum %= mod;
        sum += pow_mod_seq((seq_t)7,  cand, mod);
        sum %= mod;
        sum += pow_mod_seq((seq_t)6,  cand, mod);
        sum %= mod;

        if (sum == 0) {
            if (seq_len == seq_cap)          /* grow the cache if needed   */
                grow_cache(seq_cap ? seq_cap << 1 : 8);
            seq_cache[seq_len++] = mod;
        }
    }

    return seq_cache[n - 1];                 /* 1‑based indexing            */
}
