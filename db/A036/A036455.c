/*
 * A036455  Numbers n such that d(d(n)) is an odd prime, where d(k) is the
 *          number of divisors of k.
 * https://oeis.org/A036455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m ≥ 1 such that d(d(m)) is an odd prime,
 *            where d(k) is the number of positive divisors of k. The code
 *            searches successive integers, computes d(m) and d(d(m)) by naive
 *            trial division, and records those m for which d(d(m)) is an odd
 *            prime. A small cache stores the sequence v...
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Preliminary state – allocated progressively as more terms are needed.  */
static seq_t *seq = NULL;          /* cache of sequence values      */
static size_t seq_cap = 0;         /* current capacity of seq array */
static size_t seq_len = 0;         /* number of terms cached        */
static idx_t  last_checked = 0;    /* largest integer inspected     */

/* Append a value to the cache, expanding it if required.  */
static void append_term(seq_t v)
{
    if (seq_len == seq_cap) {
        seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = v;
}

/* Return true iff x ≥ 2, odd, and prime using straightforward trial
   division – adequate because d(d(m)) is always small.  */
static bool is_odd_prime(uint64_t x)
{
    if (x < 2 || (x & 1UL) == 0)   /* even numbers are not allowed (except 2) */
        return false;

    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0)
            return false;
    }
    return true;
}

/* Compute the number of divisors of n via trial division.  For the
   intended range (n up to the ℓ‑th term with ℓ ≤ 10000) this is
   fast enough; each divisor count involves ≤ √n divisions.  */
static uint64_t divisor_count(uint64_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    uint64_t cnt = 1;
    /* Count factor 2 */
    uint64_t exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        exp++;
    }
    if (exp > 0)
        cnt *= (exp + 1);

    /* Count odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            cnt *= (exp + 1);
        }
    }
    if (n > 1)          /* remaining prime factor */
        cnt *= 2;

    return cnt;
}

/* The required function – external linkage and signature defined by
   the build system.  It guarantees that seq contains at least n terms
   and returns the n‑th element (1‑based).  */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains the requested term.  */
    while (seq_len < n) {
        ++last_checked;

        /* d(m) and d(d(m)) */
        uint64_t d_m   = divisor_count((uint64_t)last_checked);
        uint64_t d_d_m = divisor_count(d_m);

        if (is_odd_prime(d_d_m))
            append_term((seq_t)last_checked);
    }

    return seq[n - 1];
}
