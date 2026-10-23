/*
 * A141303  Primes of the form 2*x^2+6*x*y-3*y^2 (as well as of the form
 *          5*x^2+10*x*y+2*y^2).
 * https://oeis.org/A141303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 823, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141303
   a(n) = the n-th prime of the form 2*x^2 + 6*x*y - 3*y^2
   (equivalently the n-th prime that is 2, 5, or congruent to 17 or 53 mod 60). */

/* The implementation generates primes by simple trial division,
   keeps the sequence values in a dynamically sized array, and returns
   the n‑th term on demand.  No external libraries are required. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* External type definitions supplied by the build system */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------ */
/* private state                                                */
/* ------------------------------------------------------------ */
static seq_t *seq_cache   = NULL;   /* cached sequence values          */
static idx_t seq_len      = 0;      /* number of cached values         */
static idx_t seq_cap      = 0;      /* allocated capacity of seq_cache */
static uint64_t cur       = 2;      /* next integer to check for prime */

/* Ensure seq_cache has room for at least 'need' elements. */
static void ensure_seq_cap(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* primality test using simple trial division (no dependencies) */
/* ------------------------------------------------------------ */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1ULL) == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return false;
    return true;
}

/* ------------------------------------------------------------ */
/* main function                                               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, but safe */

    /* Generate terms until we have at least n of them.  */
    while (seq_len < n) {
        /* Test the current candidate for primality. */
        if (is_prime_uint64(cur)) {
            if (cur == 2ULL || cur == 5ULL ||
                (cur % 60ULL == 17ULL) || (cur % 60ULL == 53ULL)) {
                /* It is a wanted prime.  Store it. */
                ensure_seq_cap(seq_len + 1);
                seq_cache[seq_len++] = (seq_t)cur;
            }
        }
        ++cur;                     /* move to next integer to test */
    }

    return seq_cache[n - 1];
}
