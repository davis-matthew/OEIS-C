/*
 * A191616  a(1) = 1; a(n) is the largest number m such that m-A085392(m) =
 *          a(n-1).
 * https://oeis.org/A191616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: full, pass, 10 terms through n = 10 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191616: a(1)=1; for n>1, a(n) is the largest m such that
 * m - A085392(m) = a(n-1), where A085392(m) is the largest non‑composite
 * divisor < m of m (i.e., 1 for a prime m, otherwise the largest prime
 * divisor of m).  The sequence is finite (10 terms).  For n exceeding
 * the computed length this function returns 0.
 */
#include <stddef.h>
#include <stdbool.h>

static seq_t largest_noncomposite_divisor(seq_t m)
{
    /* m is small (≤ 2*prev+1 for the relevant prev), safe to cast to uint64_t */
    uint64_t val = (uint64_t)m;
    if (val <= 2) return (seq_t)1;

    /* check primeness */
    bool isPrime = true;
    for (uint64_t d = 2; d * d <= val; ++d) {
        if (val % d == 0) { isPrime = false; break; }
    }
    if (isPrime) return (seq_t)1;

    /* composite: find largest prime factor */
    uint64_t largest = 1;
    uint64_t n = val;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            largest = d;
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1) largest = n;
    return (seq_t)largest;
}

static seq_t next_term(seq_t prev)
{
    if (prev == 0) return (seq_t)0;
    uint64_t diff = (uint64_t)prev;
    uint64_t upper = 2 * diff + 1;      /* upper bound for m (m ≤ 2*diff+1)   */
    seq_t best = 0;
    for (uint64_t m = upper; m > diff; --m) {
        seq_t sub = largest_noncomposite_divisor((seq_t)m);
        if ((seq_t)m - sub == prev) { best = (seq_t)m; break; } /* largest found */
    }
    return best;
}

static seq_t seq_cache[64];   /* enough to hold the 10-term sequence */
static size_t seq_len = 0;    /* number of terms stored (index 0 unused) */
static bool seq_initialized = false;

seq_t a(idx_t n)
{
    if (!seq_initialized) {
        /* build the sequence once */
        seq_cache[1] = (seq_t)1;
        seq_len = 1;
        while (true) {
            seq_t nxt = next_term(seq_cache[seq_len]);
            if (nxt == 0) break;      /* sequence terminates */
            seq_len++;
            seq_cache[seq_len] = nxt;
        }
        seq_initialized = true;
    }

    if (n == 0) return (seq_t)0;
    if ((idx_t)seq_len < n) return (seq_t)0; /* out of bounds */
    return seq_cache[(size_t)n];
}
