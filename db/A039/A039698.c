/*
 * A039698  Numbers k such that phi(k) + 1 is prime.
 * https://oeis.org/A039698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 26196, u32 n <= 26197, u64 n <= 26197,
 *            u128 n <= 26197, big any n
 * Validated: standard, pass, 87 terms through n = 9986 (2026-11-03)
 * Helpers:   build_spf, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence: numbers k such that φ(k)+1 is prime. */
/* The function builds the sequence lazily, using a smallest‑prime‑factor sieve
 * and a prime test for the value φ(k)+1.  The array of terms is extended
 * only as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"
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
