/*
 * A061900  Triangular numbers that are not squarefree.
 * https://oeis.org/A061900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061900
 * Formula: a(n) = the nth triangular number that is not squarefree.
 *            T_k = k*(k+1)/2, a(n) = the nth T_k with a square factor.
 * The code generates triangular numbers one by one, tests square‑freeness
 * by trial division using a growing list of primes, and memoises the
 * results so that any call to a(idx_t n) for n ≥ 1 returns the correct
 * triangular number efficiently.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- integer square root for seq_t -------------------------- */
static seq_t
isqrt_seq(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo <= hi) {
        seq_t mid = (lo + hi) >> 1;
        if (mid == 0) { lo = 1; continue; }
        if (mid > x / mid) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

/* ---------- prime table (extendable) ------------------------------ */
static unsigned *primes = (unsigned *)0;
static size_t primes_len   = 0;
static size_t primes_cap   = 0;

/* Ensure primes[] contains all primes ≤ limit.  Rebuild the sieve.   */
static void
ensure_primes_up_to(size_t limit)
{
    if (limit <= primes_cap) return;

    size_t size = limit + 1;
    bool *is_p = (bool *)calloc(size, sizeof(bool));
    if (!is_p) return;                 /* silently ignore out of memory */

    for (size_t i = 2; i < size; ++i) is_p[i] = true;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_p[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_p[j] = false;

    free(primes);
    primes = (unsigned *)malloc(size * sizeof(unsigned));
    if (!primes) { free(is_p); return; }

    primes_len = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_p[i]) primes[primes_len++] = (unsigned)i;

    primes_cap = size;
    free(is_p);
}

/* ---------- check square‑free for a triangular number -------------- */
static bool
is_squarefree(seq_t t)
{
    size_t sqrt_t = (size_t)isqrt_seq(t) + 1;
    ensure_primes_up_to(sqrt_t);

    for (size_t i = 0; i < primes_len; ++i) {
        seq_t p  = (seq_t)primes[i];
        seq_t pp = p * p;
        if (pp > t) break;
        if (t % pp == 0) return false; /* square factor found */
    }
    return true;
}

/* ---------- cache of found non‑squarefree triangular numbers ------- */
static seq_t *tri_non_sqf = (seq_t *)0;
static size_t   tri_len   = 0;
static size_t   tri_cap   = 0;
static idx_t    last_k    = 0;      /* last k processed */

/* Ensure capacity for a new entry of size new_len. */
static void
ensure_tri_cap(size_t new_len)
{
    if (new_len <= tri_cap) return;
    size_t new_cap = tri_cap ? tri_cap * 2 : 16;
    while (new_cap < new_len) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(tri_non_sqf, new_cap * sizeof(seq_t));
    if (tmp) {
        tri_non_sqf = tmp;
        tri_cap = new_cap;
    }
}

/* ---------- sequence function ------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but safe */

    while (tri_len < n) {
        ++last_k;
        seq_t t = (seq_t)last_k * ((seq_t)last_k + 1) / 2;
        if (!is_squarefree(t)) {
            ensure_tri_cap(tri_len + 1);
            tri_non_sqf[tri_len++] = t;
        }
    }
    return tri_non_sqf[n - 1];
}
