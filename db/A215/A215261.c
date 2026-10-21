/*
 * A215261  Write down the nonsemiprime numbers 1, 2, 3, 5, 7, 8, 11, 12, 13,
 *          16, 17, ... and insert between two nonsemiprimes the smallest
 *          semiprime not yet present in the sequence such that two
 *          neighboring integers sum to a nonsemiprime.
 * https://oeis.org/A215261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is a permutation of the natural numbers produced * by
 *            taking the non‑semiprimes in increasing order and, between every
 *            * pair of consecutive non‑semiprimes, inserting the smallest
 *            unused * semiprime such that both sums with its two neighbours
 *            are * non‑semiprime. No closed formula exists; th...
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* ----------  constants & global state  ---------- */
#define LIMIT        2000000        /* enough for n up to 10 000 */

static idx_t *spf;            /* smallest prime factor for each ≤ LIMIT */
static bool *used;            /* numbers already in sequence   */
static seq_t *semiprimes;     /* list of all semiprimes ≤ LIMIT */
static size_t semip_len;
static uint64_t *primes;      /* primes up to LIMIT          */
static size_t prime_cnt;

static seq_t *seq;            /* memoised sequence            */
static size_t seq_len, seq_cap;
static idx_t cur_non_candidate = 3;   /* next potential non‑semiprime */

/* Ensure seq buffer has room for at least 'min_cap' elements */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap < min_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < min_cap) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Initialise global arrays (Sieve, primes, semiprimes, sequence) */
static void init(void)
{
    /* SPF sieve up to LIMIT (O(LIMIT log log LIMIT)) */
    spf = (idx_t *)calloc(LIMIT + 1, sizeof(idx_t));
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }

    /* prime list for trial division */
    build_primes_upto(LIMIT, &primes, &prime_cnt);

    /* pre‑compute all semiprimes ≤ LIMIT */
    used = (bool *)calloc(LIMIT + 1, sizeof(bool));
    semip_len = 0;
    size_t cap = 128;
    semiprimes = (seq_t *)malloc(cap * sizeof(seq_t));
    for (idx_t x = 4; x <= LIMIT; ++x) {
        idx_t t = x;
        int cnt = 0;
        while (t > 1 && cnt < 3) {
            t /= spf[t];
            ++cnt;
        }
        if (t == 1 && cnt == 2) {
            if (semip_len == cap) {
                cap <<= 1;
                semiprimes = (seq_t *)realloc(semiprimes, cap * sizeof(seq_t));
            }
            semiprimes[semip_len++] = (seq_t)x;
        }
    }

    /* initialise sequence with the first two non‑semiprimes 1,2 */
    seq_cap = 16;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;
    seq[seq_len++] = (seq_t)1;
    seq[seq_len++] = (seq_t)2;
    used[1] = true;
    used[2] = true;
}

/* Test if n is a semiprime (product of two primes, counting multiplicity) */
static bool is_semiprime_generic(seq_t n)
{
    if (n < 4) return false;
    if (n <= LIMIT) {
        int cnt = 0;
        idx_t t = (idx_t)n;
        while (t > 1) {
            t /= spf[t];
            ++cnt;
            if (cnt > 2) return false;
        }
        return cnt == 2;
    }
    /* trial division for numbers > LIMIT */
    int cnt = 0;
    seq_t m = n;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        seq_t pp = (seq_t)p * (seq_t)p;
        if (pp > m) break;
        while (m % p == 0) {
            m /= p;
            ++cnt;
            if (cnt > 2) return false;
        }
    }
    if (m > 1) ++cnt;
    return cnt == 2;
}

/* Build the sequence until it contains at least n+1 terms (so that a(n)
 * is available). The sequence is memoised; subsequent calls will be
 * instantaneous. */
static void build_until(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init(); initialized = true; }

    /* Need at least n+1 terms to supply a(n) */
    while (seq_len <= n) {
        seq_t a = seq[seq_len - 2];
        seq_t b = seq[seq_len - 1];

        /* find smallest unused semiprime that keeps a+S and b+S
           non‑semiprime */
        seq_t chosen = 0;
        for (size_t j = 0; j < semip_len; ++j) {
            seq_t cand = semiprimes[j];
            if (used[(size_t)cand]) continue;
            seq_t s1 = a + cand;
            seq_t s2 = b + cand;
            if (!is_semiprime_generic(s1) && !is_semiprime_generic(s2)) {
                chosen = cand;
                break;
            }
        }
        if (!chosen) return;   /* safety: should never happen for the given limits */

        /* insert chosen between a and b */
        size_t pos_b = seq_len - 1;
        ensure_seq_cap(seq_len + 1);
        for (size_t k = seq_len; k > pos_b; --k)
            seq[k] = seq[k-1];
        seq[pos_b] = chosen;
        ++seq_len;
        if (chosen <= LIMIT) used[(size_t)chosen] = true;

        /* append next unused non‑semiprime */
        while (true) {
            if (cur_non_candidate > LIMIT) break;   /* safety */
            idx_t cand = cur_non_candidate;
            if (!is_semiprime_generic((seq_t)cand) && !used[(size_t)cand]) {
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = (seq_t)cand;
                if (cand <= LIMIT) used[(size_t)cand] = true;
                ++cur_non_candidate;
                break;
            }
            ++cur_non_candidate;
        }
    }
}

/* Exported function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    build_until(n);
    return seq[n - 1];
}
