/*
 * A090698  Primes of the form 2*n^2+1.
 * https://oeis.org/A090698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 2*k² + 1, found by testing
 *            2*k²+1 for primality and collecting the primes
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 3255, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin primality test for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;

    /* Small primes */
    static const uint64_t small_primes[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* Write n‑1 as d · 2ˢ with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit numbers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        unsigned __int128 x = 1, pow = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }
        uint64_t x64 = (uint64_t)x;
        if (x64 == 1 || x64 == n - 1) continue;

        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            x64 = (uint64_t)x;
            if (x64 == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----- memoised storage for the sequence ----- */
static seq_t *seq   = NULL;  /* array of computed terms   */
static size_t seq_len = 0;   /* number of terms stored    */
static size_t seq_cap = 0;   /* capacity of the array     */
static idx_t  cur_n   = 0;   /* next natural candidate index to test */

/* enlarge the array when required */
static void grow_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory – terminate program */
    seq   = tmp;
    seq_cap = new_cap;
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 – harmless */

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        ++cur_n; /* next natural index to test */

        /* candidate = 2*cur_n^2 + 1  (computed using 64‑bit arithmetic) */
        uint64_t cand64 = 2ULL * (uint64_t)cur_n * (uint64_t)cur_n + 1ULL;

        if (is_prime_u64(cand64)) {
            if (seq_len == seq_cap) grow_seq();
            seq[seq_len++] = (seq_t)cand64;
        }
    }
    return seq[n - 1];
}
