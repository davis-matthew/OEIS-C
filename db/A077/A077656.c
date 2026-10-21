/*
 * A077656  Numbers having a different number of prime factors as their
 *          successors (counted with multiplicity).
 * https://oeis.org/A077656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that Ω(n) ≠ Ω(n+1), where Ω counts prime factors
   of n with multiplicity (Ω(1)=0).  The code increments n and tests the
   condition using trial‑division; the result is cached so that any order of
   calls works. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static storage ---------- */
static idx_t   current_x       = 1; /* next integer to test   */
static idx_t   terms_cap       = 0; /* capacity of terms array */
static idx_t   terms_len       = 0; /* number of terms stored */
static seq_t  *terms           = NULL; /* a(n) values, 0‑based */

static idx_t   primes_cap      = 0; /* capacity of primes array */
static idx_t   primes_len      = 0; /* number of primes stored */
static idx_t   primes_last     = 0; /* largest prime already tested */
static idx_t  *primes          = NULL; /* list of primes in increasing order */

/* ---------- helpers for dynamic arrays ---------- */
static void ensure_terms_cap(idx_t needed)
{
    if (needed <= terms_cap) return;
    idx_t new_cap = terms_cap ? (terms_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!newp) abort();
    terms = newp;
    terms_cap = new_cap;
}

static void ensure_primes_cap(idx_t needed)
{
    if (needed <= primes_cap) return;
    idx_t new_cap = primes_cap ? (primes_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *newp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    if (!newp) abort();
    primes = newp;
    primes_cap = new_cap;
}

/* ---------- simple integer square‑root (floor) ---------- */
static idx_t idx_isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;   /* floor(sqrt(n)) */
}

/* ---------- prime generation up to a limit ---------- */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_last) return;
    for (idx_t cand = primes_last + 1; cand <= limit; ++cand) {
        bool prime = true;
        if (cand < 2) prime = false;
        else {
            idx_t sqrt_cand = idx_isqrt(cand);
            for (idx_t i = 0; i < primes_len && primes[i] <= sqrt_cand; ++i) {
                if (cand % primes[i] == 0) { prime = false; break; }
            }
        }
        if (prime) {
            ensure_primes_cap(primes_len + 1);
            primes[primes_len++] = cand;
        }
    }
    primes_last = limit;
}

/* ---------- compute Ω(n) with trial division ---------- */
static uint8_t omega_of(idx_t n)
{
    if (n <= 1) return 0;
    uint8_t count = 0;
    idx_t remaining = n;
    idx_t sqrt_n = idx_isqrt(remaining);
    ensure_primes_upto(sqrt_n);
    for (idx_t i = 0; i < primes_len && primes[i] <= sqrt_n; ++i) {
        idx_t p = primes[i];
        while (remaining % p == 0) {
            ++count;
            remaining /= p;
            if (remaining == 1) break;
        }
    }
    if (remaining > 1) ++count;  /* remaining prime factor > sqrt_n */
    return count;
}

/* ---------- generate terms until we have at least n_needed ---------- */
static void generate_terms(idx_t n_needed)
{
    while (terms_len < n_needed) {
        uint8_t w1 = omega_of(current_x);
        uint8_t w2 = omega_of(current_x + 1);
        if (w1 != w2) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)current_x;
        }
        ++current_x;
    }
}

/* ---------- required public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset guard – sequence starts at n=1 */
    generate_terms(n);
    return terms[n - 1];           /* terms[0] = a(1) */
}
