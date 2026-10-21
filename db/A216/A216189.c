/*
 * A216189  Numbers n whose odd Collatz steps (except for 1) are all primes.
 * https://oeis.org/A216189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_power_of_two, is_prime_big, seq_alloc_new_array,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: Numbers n>0 whose odd Collatz steps (excluding n itself
 * and the final 1) are all primes.  Powers of two are excluded.  The
 * function searches for terms incrementally and caches them for reuse. */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/is_power_of_two.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* ------------------------------------------------------------------ */
/* Test whether a candidate n satisfies the definition.  The function
 * works with any seq_t that supports the operators used below.  It
 * distinguishes between odd and even starting values: if n itself is
 * odd, the first odd number encountered (which is n) is not checked
 * for primality; all subsequent odd numbers (except 1) must be prime. */
static bool is_good(seq_t n)
{
    /* Powers of two are excluded. */
    if (is_power_of_two(n))
        return false;

    bool skip_first = (n & 1) != 0;   /* skip check if n is odd */
    bool first_odd_done = false;      /* flag for the first odd seen */

    seq_t x = n;
    while (x > (seq_t)1) {
        if ((x & 1) != 0) { /* odd step */
            if (!(skip_first && !first_odd_done)) {
                if (!is_prime_big(x))
                    return false;
            }
            first_odd_done = true;
            x = 3 * x + 1;
        } else {            /* even step: strip all factors of two */
            while ((x & 1) == 0)
                x = x >> 1;
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Cache of computed terms. */
static seq_t   *terms   = NULL;  /* dynamic array of seq_t       */
static size_t  count   = 0;      /* number of stored terms       */
static size_t  capacity = 0;     /* size of allocated buffer     */
static idx_t   checked  = 0;     /* last integer inspected       */

/* Grow to at least new_cap and copy old data. */
static void grow_cache(size_t new_cap)
{
    seq_t *new_terms = seq_alloc_new_array(new_cap);
    if (terms) {
        for (size_t i = 0; i < count; ++i)
            new_terms[i] = terms[i];
        seq_free_array(terms);
    }
    terms = new_terms;
    capacity = new_cap;
}

/* Append a new term to the cache, growing the buffer when needed. */
static void add_term(seq_t v)
{
    if (count == capacity)
        grow_cache(capacity ? capacity << 1 : 16);

    terms[count++] = v;
}

/* Ensure that the first n terms have been generated. */
static void ensure_terms(idx_t n)
{
    if (n <= count)
        return;

    if (checked < 3)
        checked = 3;               /* first admissible candidate */

    while (count < n) {
        seq_t cand = (seq_t)checked++;
        if (is_good(cand))
            add_term(cand);
    }
}

/* ------------------------------------------------------------------ */
/* Main entry point for the sequence A216189. */
seq_t a(idx_t n)
{
    /* a(1) is the first term (3). */
    ensure_terms(n);
    return terms[n - 1];
}
