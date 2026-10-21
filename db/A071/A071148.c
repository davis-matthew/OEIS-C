/*
 * A071148  Partial sums of sequence of odd primes (A065091); a(n) = sum of
 *          the first n odd primes.
 * https://oeis.org/A071148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the first n odd primes (3,5,7,…)
 * Range:     u8 n <= 12, u16 n <= 156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ----------------- static state --------------------------------- */
static seq_t *primes = NULL;     /* odd primes themselves       */
static seq_t *sums   = NULL;     /* partial sums of odd primes  */
static idx_t  count   = 0;       /* number of primes stored     */
static idx_t  capacity = 0;      /* allocated array size        */
static idx_t  last_candidate = 1;/* next odd number to test     */

/* ----------------- helpers -------------------------------------- */
static void grow_arrays(idx_t new_cap)
{
    if (new_cap <= capacity)
        return;
    idx_t new_capacity = (capacity == 0) ? 16 : capacity * 2;
    while (new_capacity < new_cap)
        new_capacity <<= 1;          /* double until large enough */
    primes = (seq_t *)realloc(primes, new_capacity * sizeof(seq_t));
    sums   = (seq_t *)realloc(sums,   new_capacity * sizeof(seq_t));
    capacity = new_capacity;
}

static bool is_prime_candidate(idx_t cand)
{
    if (cand < 3)
        return false;
    for (idx_t i = 0; i < count; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p * p > cand)                /* trial division bound */
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

static void ensure_primes(idx_t n)
{
    if (count >= n)
        return;
    grow_arrays(n);
    while (count < n) {
        last_candidate += 2;            /* next odd integer */
        if (is_prime_candidate(last_candidate)) {
            primes[count] = (seq_t)last_candidate;
            sums[count] = (count == 0) ? (seq_t)last_candidate
                                       : sums[count-1] + (seq_t)last_candidate;
            ++count;
        }
    }
}

/* ----------------- required function ----------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for index 0 – return 0 */
        return 0;
    ensure_primes(n);     /* make sure we have at least n primes */
    return sums[n-1];     /* sums[0] = 3, sums[1] = 8, … */
}
