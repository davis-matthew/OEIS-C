/*
 * A086469  Sum of the distinct (smallest) prime signature divisors of n. In
 *          case of two or more divisors with the same prime signature the
 *          smallest is considered to evaluate the sum. Let this function be
 *          defined as psigma(n).
 * https://oeis.org/A086469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the distinct (smallest) prime‑signature divisors
 *            of n. The function implements the definition of the sequence
 *            A086469
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sort_ints
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper functions are declared static so that they do not
   clash with names that may already exist in the harness. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/sort_ints.h"

/* ------------------------------------------------------------------ */
/* factorise n into distinct primes and their exponents.  The arrays
   primes[] and exps[] have room for at least 20 entries; pcnt is the
   number of distinct primes. */
static void factor_uint64(uint64_t n, uint64_t *primes, int *exps, size_t *pcnt)
{
    size_t i = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            int e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            primes[i] = d;
            exps[i]   = e;
            ++i;
        }
    }
    if (n > 1) {
        primes[i] = n;
        exps[i]   = 1;
        ++i;
    }
    *pcnt = i;
}

/* ------------------------------------------------------------------ */
/* Generate all divisors of a number given its prime factorisation.
   Recursively builds the divisors list in divs[0..cnt-1].  The arrays
   primes[0..pcnt-1] and exps[0..pcnt-1] describe the factorisation.   */
static void gen_divs_rec(const uint64_t *pr, const int *exp, size_t pcnt,
                         size_t idx, uint64_t cur, uint64_t *divs, size_t *cnt)
{
    if (idx == pcnt) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divs_rec(pr, exp, pcnt, idx + 1, val, divs, cnt);
        if (e < exp[idx]) val *= pr[idx];
    }
}

static void gen_divs(const uint64_t *primes, const int *exps, size_t pcnt,
                     uint64_t *divs, size_t *cnt)
{
    *cnt = 0;
    gen_divs_rec(primes, exps, pcnt, 0, 1ULL, divs, cnt);
}

/* ------------------------------------------------------------------ */

/* Signatures are stored in this structure.  We keep the sorted
   list of exponents and the smallest divisor that exhibits the
   signature. */
typedef struct {
    int  len;          /* number of exponents (0 for the divisor 1) */
    int  exps[20];     /* the exponents, sorted */
    uint64_t mindiv;   /* smallest divisor with this signature */
} sig_entry_t;

/* ------------------------------------------------------------------ */
/* Compare two signatures for equality.  They must have the same length. */
static bool same_sig(const sig_entry_t *s1, const sig_entry_t *s2)
{
    if (s1->len != s2->len) return false;
    for (int i = 0; i < s1->len; ++i) {
        if (s1->exps[i] != s2->exps[i]) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main function as required by the harness.  Computes a(n) for any n>=1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    uint64_t nn = (uint64_t)n;     /* n fits in uint64_t for the factorisation */

    /* ---------- factorise n --------------------------------------- */
    uint64_t primes[20];
    int       exps  [20];
    size_t    pcnt;
    factor_uint64(nn, primes, exps, &pcnt);

    /* ---------- total number of divisors --------------------------- */
    size_t maxdiv = 1;
    for (size_t i = 0; i < pcnt; ++i) maxdiv *= (exps[i] + 1);

    /* ---------- generate all divisors ------------------------------ */
    uint64_t *divs = (uint64_t *)malloc(maxdiv * sizeof(uint64_t));
    size_t    dcnt;
    gen_divs(primes, exps, pcnt, divs, &dcnt);

    /* ---------- collect minimal divisor per signature ---------------- */
    sig_entry_t *sig_arr = (sig_entry_t *)malloc(dcnt * sizeof(sig_entry_t));
    size_t       sig_cnt = 0;

    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d  = divs[i];
        int      exps_s[20];
        int      len   = 0;
        uint64_t tmp   = d;

        /* compute exponents of the primes of n that appear in d */
        for (size_t j = 0; j < pcnt; ++j) {
            int e = 0;
            while (tmp % primes[j] == 0) {
                tmp /= primes[j];
                ++e;
            }
            if (e > 0) {
                exps_s[len++] = e;
            }
        }
        sort_ints(exps_s, len);

        /* search for an existing signature */
        size_t pos = sig_cnt;
        for (size_t k = 0; k < sig_cnt; ++k) {
            if (sig_arr[k].len == len) {
                bool eq = true;
                for (int t = 0; t < len; ++t) {
                    if (sig_arr[k].exps[t] != exps_s[t]) { eq = false; break; }
                }
                if (eq) { pos = k; break; }
            }
        }

        if (pos == sig_cnt) {          /* new signature */
            sig_arr[sig_cnt].len     = len;
            for (int t = 0; t < len; ++t) sig_arr[sig_cnt].exps[t] = exps_s[t];
            sig_arr[sig_cnt].mindiv  = d;
            ++sig_cnt;
        } else {                        /* existing signature, keep smaller divisor */
            if (d < sig_arr[pos].mindiv) sig_arr[pos].mindiv = d;
        }
    }

    /* ---------- sum the minimal divisors per signature ------------- */
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k < sig_cnt; ++k) {
        sum += (seq_t)sig_arr[k].mindiv;
    }

    /* ---------- clean up ------------------------------------------- */
    free(divs);
    free(sig_arr);

    return sum;
}
