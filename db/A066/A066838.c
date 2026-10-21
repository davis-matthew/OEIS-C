/*
 * A066838  Product of primes < n that do not divide n.
 * https://oeis.org/A066838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 30, u64 n <= 58, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A066838 – Product of primes < n that do not divide n.
 *
 *  a(n) = ∏ { p : p prime, p < n, n mod p ≠ 0 }
 *
 *  The function generates primes by a sieve that grows incrementally
 *  as larger arguments are requested.  For each call it multiplies
 *  the primes that are not divisors of n.
 *
 *  The implementation avoids static layout of huge tables and keeps
 *  all allocations dynamic so that it works for the arbitrary‑precision
 *  build as well as for the 64‑bit build.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *primes_arr      = NULL;   /* array of found primes       */
static size_t   primes_cap      = 0;        /* capacity of primes_arr      */
static size_t   primes_cnt      = 0;        /* number of primes stored     */

/* sieve array: 0 → prime, 1 → composite for numbers 0 .. max_sieved */
static uint8_t *sieve          = NULL;
static idx_t    max_sieved     = 0;        /* highest number already sieved */

/* ------------------------------------------------------------------
 *  enlarge_primes_array: ensure the prime array can hold at least
 *  new_cap entries.
 */
static void enlarge_primes_array(size_t new_cap)
{
    if (new_cap <= primes_cap) return;
    uint64_t *tmp = (uint64_t*)realloc(primes_arr, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);          /* allocation failure */
    primes_arr = tmp;
    primes_cap = new_cap;
}

/* ------------------------------------------------------------------
 *  ensure_primes(limit)  – guarantee that all primes ≤ limit are
 *  known and stored in primes_arr.
 *
 *  The function builds the sieve incrementally.  It keeps the
 *  already sieved part unchanged and only processes the newly
 *  requested interval.
 */
static void ensure_primes(idx_t limit)
{
    if (limit <= max_sieved) return;          /* nothing to do */

    /* ---------- resize sieve array ----------
     * Expand the sieve array to (limit + 1) entries.
     */
    uint8_t *tmp = (uint8_t*)realloc(sieve, (limit + 1) * sizeof(uint8_t));
    if (!tmp) exit(EXIT_FAILURE);
    /* Initialise the new part to 0 (candidate prime). */
    memset(tmp + max_sieved + 1, 0, (limit - max_sieved) * sizeof(uint8_t));
    sieve = tmp;

    /* ---------- cross‑mark composites using already known small primes ----------
     */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes_arr[pi];
        if (p * p > limit) break;
        idx_t start = (max_sieved < p * p) ? (p * p) : ((max_sieved + p) / p) * p;
        for (idx_t j = start; j <= limit; j += p) {
            sieve[j] = 1;
        }
    }

    /* ---------- run sieve on the new interval ----------
     * Find new primes and mark their multiples.
     */
    for (idx_t j = max_sieved + 1; j <= limit; ++j) {
        if (j < 2) continue;              /* 0 and 1 are not prime */
        if (!sieve[j]) {
            /* new prime found */
            enlarge_primes_array(primes_cnt + 1);
            primes_arr[primes_cnt++] = j;

            /* mark multiples of j in the new interval */
            idx_t start = j * j;
            if (start > limit) continue;
            for (idx_t k = start; k <= limit; k += j) {
                sieve[k] = 1;
            }
        }
    }

    max_sieved = limit;
}

/* ------------------------------------------------------------------
 *  public routine: compute the sequence value for a given index.
 */
seq_t a(idx_t n)
{
    /* For n <= 1 the product of an empty set is defined as 1.  */
    if (n <= 1) return (seq_t)1;

    /* Prepare the list of primes less than n. */
    ensure_primes(n - 1);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < primes_cnt && primes_arr[i] < n; ++i) {
        if (n % primes_arr[i] != 0) {
            prod *= (seq_t)primes_arr[i];
        }
    }
    return prod;
}
