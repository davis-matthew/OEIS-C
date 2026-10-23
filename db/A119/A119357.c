/*
 * A119357  Numbers k for which there exist two distinct subsets of the set of
 *          divisors of k having the same sum.
 * https://oeis.org/A119357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119357:  k such that two distinct subsets of div(k) have the same sum. */
/* This implementation builds the sequence on demand by testing each integer for the
 * collision property using a subset‑sum bit‑set DP. The function a(n) returns the n‑th
 * term (a(1)=6). It is memoised so that calls with arbitrary n values are handled
 * efficiently. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------- auxiliary structures for memoisation ---------- */
static seq_t *seq_vals = NULL;
static size_t  seq_size = 0;          /* number of terms already computed      */
static uint64_t last_checked = 1;    /* last integer that has been examined   */
static bool     seq_inited   = false;

/* ---------- bit‑set storage for subset‑sum DP ---------- */
static uint64_t *reach   = NULL;     /* current reachable sums                */
static uint64_t *tmp     = NULL;     /* temporary shift results                */
static size_t    bit_cap_words = 0;   /* capacity in 64‑bit words              */
static bool      bits_inited   = false;

/* ---------- factorisation (trial division) ---------- */
static void factorize(uint64_t n, uint64_t *pf, int *pe, size_t *cnt)
{
    *cnt = 0;
    uint64_t t = n;
    for (uint64_t d = 2; d * d <= t; ++d) {
        if (t % d == 0) {
            int e = 0;
            while (t % d == 0) { t /= d; ++e; }
            pf[(*cnt)] = d;
            pe[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (t > 1) {
        pf[(*cnt)] = t;
        pe[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ---------- generate all divisors from prime factors ---------- */
static void generate_divisors(const uint64_t *pf, const int *pe,
                              size_t pcnt, uint64_t *divs, size_t *dcnt)
{
    divs[0] = 1;
    *dcnt  = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        size_t prev = *dcnt;
        uint64_t power = 1;
        for (int k = 1; k <= pe[i]; ++k) {
            power *= pf[i];
            for (size_t j = 0; j < prev; ++j) {
                divs[(*dcnt)] = divs[j] * power;
                ++(*dcnt);
            }
        }
    }
}

/* ---------- test whether a number has a collision among its divisor sums ---------- */
static bool has_collision(uint64_t n)
{
    /* factorise */
    uint64_t pf[32];
    int     pe[32];
    size_t  pcnt;
    factorize(n, pf, pe, &pcnt);

    /* build divs */
    uint64_t divs[256];
    size_t dcnt;
    generate_divisors(pf, pe, pcnt, divs, &dcnt);

    /* compute maximum possible sum (sum of all divisors) */
    uint64_t sigma = 0;
    for (size_t i = 0; i < dcnt; ++i) sigma += divs[i];
    size_t words = (sigma >> 6) + 1;  /* number of 64‑bit words needed */

    /* expand bit arrays if needed */
    if (words > bit_cap_words) {
        free(reach);
        free(tmp);
        reach = (uint64_t*)malloc(sizeof(uint64_t) * words);
        tmp   = (uint64_t*)malloc(sizeof(uint64_t) * words);
        bit_cap_words = words;
    }

    /* initialise reachable[0] = 1, others 0 */
    memset(reach, 0, sizeof(uint64_t) * words);
    reach[0] = 1;

    /* subset‑sum DP with early collision detection */
    for (size_t idx = 0; idx < dcnt; ++idx) {
        uint64_t d = divs[idx];
        size_t   shift_words = d >> 6;
        unsigned shift_bits  = d & 63;

        /* compute tmp = reach << d */
        for (size_t i = words; i-- > shift_words;) {
            uint64_t src  = reach[i - shift_words];
            uint64_t val  = src << shift_bits;
            if (shift_bits && i - shift_words >= 1) {
                val |= reach[i - shift_words - 1] >> (64 - shift_bits);
            }
            if (val & reach[i]) return true;   /* collision detected */
            tmp[i] = val;
        }
        /* merge new sums into reach */
        for (size_t i = shift_words; i < words; ++i) reach[i] |= tmp[i];
    }
    /* no collision found */
    return false;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise memoisation structures on first call */
    if (!seq_inited) {
        seq_inited = true;
        seq_vals   = NULL;
        seq_size   = 0;
        last_checked = 1;
    }
    if (!bits_inited) {
        bits_inited = true;
        reach = NULL;
        tmp   = NULL;
        bit_cap_words = 0;
    }

    if (n == 0) return (seq_t)0;   /* undefined for n=0 */

    /* fetch from cache if already computed */
    if ((idx_t)seq_size >= n) return seq_vals[n-1];

    /* extend sequence until we have n terms */
    while (seq_size < n) {
        ++last_checked;
        if (has_collision(last_checked)) {
            /* store the new term */
            seq_vals = (seq_t*)realloc(seq_vals, sizeof(seq_t) * (seq_size + 1));
            seq_vals[seq_size] = (seq_t)last_checked;
            ++seq_size;
        }
    }
    return seq_vals[n-1];
}
