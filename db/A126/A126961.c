/*
 * A126961  Primes p such that (3p)^2 + 2 is not prime.
 * https://oeis.org/A126961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that (3p)^2 + 2 is not prime */

#include <stddef.h>
#include "../utils/isPrimeGeneric.h"  /* isPrimeGeneric(seq_t) */
#include "../utils/is_prime.h"       /* is_prime(idx_t) */

#define MAX_TERMS 20000   /* sufficient for n ≤ 10 000 */

static seq_t  results[MAX_TERMS];   /* stored primes that satisfy the condition */
static size_t results_len = 0;      /* number of primes stored so far */
static idx_t  last_checked = 0;     /* last integer examined in the search */

/* Ensure that at least 'need' terms are available. */
static void
ensure_terms(size_t need)
{
    if (results_len >= need) return;

    for (;;)
    {
        idx_t cand = last_checked + 1;
        last_checked = cand;

        /* skip non‑prime candidates */
        if (!is_prime(cand))
            continue;

        /* compute (3p)^2 + 2 = 9*p*p + 2 */
        seq_t val = (seq_t)cand;
        val *= val;            /* p²   */
        val *= (seq_t)9;       /* 9p² */
        val += (seq_t)2;       /* +2  */

        /* test the condition */
        if (!isPrimeGeneric(val))
        {
            if (results_len < MAX_TERMS)
                results[results_len++] = (seq_t)cand;
            /* else ignore – our static buffer is full */
        }

        if (results_len >= need)
            break;   /* enough terms accumulated */
    }
}

/* Return the n‑th term of A126961 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n < 1 */
    if (n > MAX_TERMS) return 0;       /* caller expects n within limits */

    ensure_terms((size_t)n);
    return results[(size_t)n - 1];
}
