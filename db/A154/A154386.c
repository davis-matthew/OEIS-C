/*
 * A154386  Primes such that number of prime digits < number of nonprime
 *          digits.
 * https://oeis.org/A154386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) is the n-th prime whose number of prime‑digit occurrences
 *            (2,3,5,7) is strictly less than the number of non‑prime digit
 *            occurrences. The sequence is generated on demand by scanning
 *            integers upward, testing primality with isPrimeGeneric() and
 *            counting digit frequencies with digit_frequencies(...
 * Range:     u8 n <= 24, u16 n <= 3699, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_frequencies, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utilities -------------------------------------------------------- */
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n) */
#include "../utils/digit_frequencies.h" /* void digit_frequencies(seq_t x, unsigned int freq[10]) */
#include "../utils/append_candidate.h" /* void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val) */

/* Static cache ----------------------------------------------------------- */
static seq_t *good      = NULL;   /* array of qualifying primes                */
static size_t good_cap  = 0;      /* capacity of the array                    */
static size_t good_sz   = 0;      /* number of elements stored                 */
static idx_t  last_tested = 0;    /* last integer inspected (inclusive)        */

/* ------------------------------------------------------------------------ */
/* Test whether a prime p has more non‑prime digits than prime digits.      */
/* Uses digit_frequencies which works for arbitrary precision seq_t.       */
static bool prime_has_more_nonprime_digits(seq_t p)
{
    unsigned int freq[10] = {0};
    digit_frequencies(p, freq);          /* freq[d] = digits of d in p */

    unsigned int prime_cnt = freq[2] + freq[3] + freq[5] + freq[7];
    unsigned int total = 0;
    for (int d = 0; d < 10; ++d) total += freq[d];

    return prime_cnt < (total - prime_cnt);
}

/* ------------------------------------------------------------------------ */
/* Grow the cache until it contains at least 'req' qualifying primes.       */
/* Scans integers incrementally, testing each for primality and the digit   */
/* condition.  The search starts from the last inspected integer so that    */
/* calls to a() are efficient even when requested in arbitrary order.      */
static void ensure_good(idx_t req)
{
    if (good_sz >= (size_t)req)
        return;                                   /* already enough */

    for (idx_t cand = last_tested + 1; good_sz < (size_t)req; ++cand, ++last_tested)
    {
        seq_t scand = (seq_t)cand;
        if (!isPrimeGeneric(scand))
            continue;                          /* not a prime */

        if (prime_has_more_nonprime_digits(scand))
            append_candidate(&good, &good_sz, &good_cap, scand);
    }
}

/* ------------------------------------------------------------------------ */
/* Main accessor for the OEIS sequence.                                     */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined; return 0 as a safe fallback */
        return 0;

    ensure_good(n);                          /* guarantee n elements */
    return good[(size_t)(n - 1)];
}
