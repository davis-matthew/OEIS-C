/*
 * A039698  Numbers k such that phi(k) + 1 is prime.
 * https://oeis.org/A039698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 9986, u32 n <= 9986, u64 n <= 9986,
 *            u128 n <= 9986, big any n
 * Helpers:   build_spf, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence: numbers k such that φ(k)+1 is prime. */
/* The function builds the sequence lazily, using a smallest‑prime‑factor sieve
 * and a prime test for the value φ(k)+1.  The array of terms is extended
 * only as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"      /* build_spf() */
#include "../utils/is_prime_big.h"  /* is_prime_big() */
#include "../utils/seq_realloc.h"   /* seq_realloc() */

/* ---------- static cache for the sequence ---------- */
static seq_t *seq      = NULL;   /* array of found terms                */
static size_t seq_cap  = 0;      /* current capacity of seq array       */
static size_t seq_len  = 0;      /* number of terms stored              */

/* ---------- static cache for smallest prime factors (spf) ---------- */
static idx_t *spf      = NULL;   /* spf[n] = smallest prime factor of n (for n≥2) */
static idx_t spf_cap   = 0;      /* largest n that spf currently covers     */
static idx_t max_k     = 0;      /* largest k processed so far              */

/* -------------------------------------------------------------------- */
/* Ensure we have spf values up to at least `limit`.                     */
/* -------------------------------------------------------------------- */
static void
ensure_spf_upto(idx_t limit)
{
    if (limit <= spf_cap)
        return;                      /* already enough */

    /* Grow the spf array to a modestly larger size. */
    idx_t new_cap = limit * 2 + 10;
    idx_t *new_spf = NULL;
    build_spf(new_cap, &new_spf);     /* new_spf[0…new_cap-1] is filled */
    free(spf);                        /* discard old buffer */
    spf      = new_spf;
    spf_cap  = new_cap;
}

/* -------------------------------------------------------------------- */
/* Compute φ(n) using the pre‑computed spf array.                        */
/* -------------------------------------------------------------------- */
static seq_t
phi_from_spf(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;              /* φ(1)=1 */

    seq_t result = (seq_t)n;
    idx_t temp   = n;

    while (temp > 1) {
        idx_t p   = spf[temp];              /* spf[temp] > 1 for temp > 1 */
        seq_t tp  = (seq_t)p;
        result    = result / tp * (tp - 1);
        while (temp % p == 0)
            temp /= p;
    }
    return result;
}

/* -------------------------------------------------------------------- */
/* Main sequence function.                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1 – undefined for 0 */

    /* Build terms until we have at least n elements. */
    while (seq_len < n) {
        ++max_k;                             /* next integer k to test */
        ensure_spf_upto(max_k);              /* make sure spf covers max_k */

        seq_t phi = phi_from_spf(max_k);
        seq_t cand = phi + (seq_t)1;          /* φ(k)+1 */

        if (is_prime_big(cand)) {
            /* grow the array if necessary */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq = seq_realloc(seq, seq_len, seq_cap);
            }
            seq[seq_len++] = (seq_t)max_k;
        }
    }
    return seq[n - 1];                     /* array is 0‑based, index n-1   */
}
