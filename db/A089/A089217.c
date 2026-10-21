/*
 * A089217  n-2 is a prime of the form 4*k+3.
 * https://oeis.org/A089217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth prime ≡ 3 (mod 4), plus 2. This is equivalent to
 *            the definition a(n) = A087915(n) + A089193(n) in OEIS
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_uint64, mulmod, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod.h"

/*---  Helper functions for 64‑bit primality testing (deterministic M-R)  ---*/

/*---  Dynamic array for computed terms  ---*/

static seq_t *terms = NULL;          /* array of computed sequence terms   */
static size_t terms_len = 0;          /* number of terms stored            */
static size_t terms_cap = 0;          /* capacity of the terms array       */

/* Ensure the terms array can hold at least 'min_cap' elements.  */
static void grow_terms(size_t min_cap)
{
    if (min_cap <= terms_cap) return;
    size_t new_cap = terms_cap > 0 ? terms_cap * 2 : 64;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/*---  Sequence generation  ---*/

static uint64_t next_prime_candidate = 1;  /* static across calls */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for 0 is irrelevant */

    size_t target = (size_t)n;
    if (target <= terms_len) return terms[target - 1];

    /* Generate terms until we have at least 'n' of them. */
    while (terms_len < target) {
        /* Find the next prime. */
        uint64_t candidate;
        do {
            next_prime_candidate++;
            candidate = next_prime_candidate;
        } while (!is_prime_uint64(candidate));

        /* If prime ≡ 3 (mod 4) store the sequence term. */
        if ((candidate & 3ULL) == 3ULL) {
            if (terms_len == terms_cap) grow_terms(terms_len + 64);
            seq_t val = (seq_t)candidate;
            val = val + (seq_t)2;   /* exact addition – works for built‑in types
                                       and for arbitrary‑precision classes that
                                       overload + */
            terms[terms_len++] = val;
        }
    }
    return terms[n - 1];
}
