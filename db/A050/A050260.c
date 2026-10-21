/*
 * A050260  Quiteprimes: numbers k such that |2*(k mod p) - p| <= p + 1 -
 *          sqrt(p) for all primes p <= sqrt(k).
 * https://oeis.org/A050260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050260: Quiteprimes  
   a(n) is the n‑th integer k (k ≥ 2) such that for every prime p ≤ √k
   the inequality
         |2·(k mod p) – p| ≤ p + 1 – √p
   holds.  The implementation uses a brute‑force search with memoisation
   of previously found terms and a growing list of primes.  It is fully
   compliant with the required function signature and the build
   constraints. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --- Helpers for integer square roots ----------------------------------- */

static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

static seq_t isqrt_seq(seq_t n)
{
    seq_t low = 0, high = n;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* --- Prime sieve (small‑scale, incremental) ----------------------------- */

static idx_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;

static void ensure_primes(idx_t limit)
{
    if (primes_len == 0) {
        primes_cap = 64;
        primes = (idx_t*)malloc(primes_cap * sizeof(idx_t));
        primes_len = 0;
    }

    if (primes_len > 0 && limit <= primes[primes_len - 1])
        return;

    idx_t start = (primes_len > 0) ? primes[primes_len - 1] + 1 : 2;
    for (idx_t cand = start; cand <= limit; ++cand) {
        idx_t root = isqrt_idx(cand);
        bool is_prime = true;
        for (size_t i = 0; i < primes_len && primes[i] <= root; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                primes = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_len++] = cand;
        }
    }
}

/* --- Check whether a candidate k satisfies the definition ----------- */

static bool satisfies_candidate(seq_t k)
{
    idx_t sqrt_k = (idx_t)isqrt_seq(k);
    ensure_primes(sqrt_k);

    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = primes[i];
        if (p > sqrt_k)
            break;

        seq_t r = k % (seq_t)p;              /* k mod p  (0 ≤ r < p)          */
        seq_t two_r = r + r;                 /* 2·r   */
        seq_t lhs;                           /* |2r – p| */
        if (two_r <= p)
            lhs = p - two_r;
        else
            lhs = two_r - p;

        /* If |2r – p| > p+1 it can never satisfy the main inequality. */
        if (lhs > (seq_t)(p + 1))
            return false;

        seq_t S = (seq_t)(p + 1) - lhs;      /* S = p + 1 – |2r – p|          */
        unsigned __int128 Ssq = (unsigned __int128)S * (unsigned __int128)S;
        /* Main inequality |2r – p| ≤ p + 1 – √p  ⇔  S² ≥ p. */
        if (Ssq < (unsigned __int128)p)
            return false;
    }
    return true;
}

/* --- Memoised generation of the sequence ------------------------------ */

static seq_t *seq_terms = NULL;
static size_t seq_len = 0, seq_cap = 0;
static seq_t next_candidate = 2;

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n terms. */
    while (seq_len < n) {
        if (satisfies_candidate(next_candidate)) {
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
                seq_terms = (seq_t*)realloc(seq_terms, seq_cap * sizeof(seq_t));
            }
            seq_terms[seq_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return seq_terms[n - 1];
}
