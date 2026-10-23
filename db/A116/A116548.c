/*
 * A116548  a(n) = smallest divisor d of n that occurs earlier in the sequence
 *          fewer than a(d) times.
 * https://oeis.org/A116548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor d of n such that cnt[d] < a(d), where
 *            cnt[d] is the number of times d has appeared in the sequence so
 *            far
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory allocation and utilities */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ---------------------------  Globals  --------------------------- */

/* 1‑indexed arrays holding the sequence and the occurrence counts.
   They are grown lazily as larger n are requested. */
static seq_t  *seq   = NULL;   /* seq[i] = a(i) */
static size_t  seq_cap = 0;    /* capacity of seq array          */
static idx_t   seq_len = 0;    /* maximum index computed so far   */

static size_t *cnts  = NULL;   /* cnts[v] = number of times v occurs in the sequence */
static size_t  cnt_cap = 0;    /* capacity of cnts array          */

/* Smallest prime factor table used for factorisation. */
static uint64_t *spf = NULL;
static idx_t   spf_cap = 0;    /* largest number for which spf is valid */

/* ---------------------------  Helpers  --------------------------- */

/* Recompute the smallest‑prime‑factor table up to `limit` (inclusive). */
static void build_spf_upto(idx_t limit)
{
    spf = (uint64_t*)realloc(spf, (size_t)(limit + 1) * sizeof(uint64_t));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    if (limit >= 1) spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_cap = limit;
}

/* Ensure all auxiliary arrays are large enough for indices up to `n`. */
static void ensure_capacity(idx_t n)
{
    /* Sequence array */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 1;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq = (seq_t*)realloc(seq, (new_cap + 1) * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* Count array */
    if ((size_t)n + 1 > cnt_cap) {
        size_t new_cap = cnt_cap ? cnt_cap : 1;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        cnts = (size_t*)realloc(cnts, (new_cap + 1) * sizeof(size_t));
        memset(&cnts[cnt_cap + 1], 0, (new_cap - cnt_cap) * sizeof(size_t));
        cnt_cap = new_cap;
    }

    /* Spf table */
    if (n > spf_cap) {
        build_spf_upto(n);
    }
}

/* Recursively enumerate all divisors of `target` (excluding target itself).
   Whenever a divisor cur satisfies cnts[cur] < seq[cur], update *min_d
   if cur is smaller.  `primes` and `exps` describe the factorisation of
   target. */
static void dfs_divisors(uint64_t cur, size_t idx, size_t np,
                         const uint64_t *primes, const uint8_t *exps,
                         uint64_t target, uint64_t *min_d,
                         seq_t *seq_arr, size_t *cnt_arr)
{
    if (idx == np) {
        if (cur == target) return;               /* skip n itself */
        size_t d_idx = (size_t)cur;
        /* seq_arr[d_idx] holds a(d) for d < target */
        if (cnt_arr[d_idx] < seq_arr[d_idx] && cur < *min_d)
            *min_d = cur;
        return;
    }
    uint64_t p = primes[idx];
    uint8_t  e = exps[idx];
    uint64_t val = cur;
    for (uint8_t j = 0; j <= e; ++j) {
        dfs_divisors(val, idx + 1, np, primes, exps,
                     target, min_d, seq_arr, cnt_arr);
        val *= p;
    }
}

/* Compute the sequence values up to index `n`. */
static void ensure_seq_up_to(idx_t n)
{
    if (seq_len >= n) return;

    ensure_capacity(n);

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t value;
        if (i == 1) {                       /* base case */
            value = (seq_t)1;
        } else if (spf[i] == i) {           /* i is prime */
            value = (seq_t)i;
        } else {                            /* composite */
            /* Factorise i using the spf table. */
            uint64_t primes[20];
            uint8_t  exps[20];
            size_t np = 0;
            uint64_t tmp = (uint64_t)i;
            while (tmp > 1) {
                uint64_t p = spf[tmp];
                uint8_t e = 0;
                while (tmp % p == 0) { tmp /= p; ++e; }
                primes[np] = p;
                exps[np] = e;
                ++np;
            }

            /* Find the minimal qualifying divisor. */
            uint64_t min_d = UINT64_MAX;
            dfs_divisors(1, 0, np, primes, exps,
                         (uint64_t)i, &min_d, seq, cnts);
            value = (seq_t)min_d;
        }

        seq[(size_t)i] = value;
        ++cnts[(size_t)value];
    }
    seq_len = n;
}

/* ---------------------------  Main function  --------------------------- */

/* External linkaged function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */
    ensure_seq_up_to(n);
    return seq[(size_t)n];
}
