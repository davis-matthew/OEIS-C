/*
 * A066502  Numbers k such that 7 divides phi(k).
 * https://oeis.org/A066502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066502: numbers k such that 7 divides φ(k).
 * This implementation finds all such numbers up to a fixed bound
 * (chosen large enough for the first 10 000 terms) by a sieve and
 * stores them in a static cache.  The function a(idx_t n) returns
 * the n‑th term (1‑based).  The buffer is allocated on first use.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------- local helper ---------- */

static seq_t *seq_cache = NULL;   /* cache of sequence terms          */
static idx_t seq_len   = 0;       /* number of cached terms           */
static bool  cache_initiated = false; /* has init_cache been run?       */

/* Produce the cache up to a bound that contains at least the first
   10 000 terms of the sequence.  The bound is 200 000 which
   comfortably exceeds the 10 000th element (the sequence has
   density →1). */
static void init_cache(void)
{
    /* --------- parameters ---------- */
    const idx_t BOUND = 200000;   /* sieve upper limit */

    /* --------- mark numbers that are multiples of 49 ---------- */
    bool *marked = (bool *)malloc((BOUND + 1) * sizeof(bool));
    if (!marked) return; /* allocation failed – abort silently */
    memset(marked, 0, (BOUND + 1) * sizeof(bool));

    for (idx_t k = 49; k <= BOUND; k += 49)
        marked[k] = true;

    /* --------- sieve primes up to BOUND ---------- */
    bool *is_prime = (bool *)malloc((BOUND + 1) * sizeof(bool));
    if (!is_prime) { free(marked); return; }
    memset(is_prime, 1, (BOUND + 1) * sizeof(bool));

    if (BOUND >= 0) is_prime[0] = false;
    if (BOUND >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= BOUND; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= BOUND; j += i)
                is_prime[j] = false;
        }
    }

    /* --------- mark multiples of primes ≡1 (mod 7) ---------- */
    for (idx_t p = 2; p <= BOUND; ++p) {
        if (is_prime[p] && p % 7 == 1) {
            for (idx_t k = p; k <= BOUND; k += p)
                marked[k] = true;
        }
    }

    free(is_prime);

    /* --------- collect the marked numbers into the cache ---------- */
    size_t count = 0;
    for (idx_t i = 2; i <= BOUND; ++i)
        if (marked[i]) ++count;

    seq_cache = (seq_t *)malloc(count * sizeof(seq_t));
    if (!seq_cache) { free(marked); return; }
    seq_len = 0;

    for (idx_t i = 2; i <= BOUND; ++i) {
        if (marked[i])
            seq_cache[seq_len++] = (seq_t)i; /* cast to seq_t */
    }

    free(marked);
    cache_initiated = true;
}

/* --------- exported sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* invalid index → 0 */

    if (!cache_initiated)
        init_cache();

    if (!cache_initiated || n > seq_len)
        return (seq_t)0;          /* out of range → 0 */

    return seq_cache[n - 1];      /* 1‑based indexing */
}
