/*
 * A024588  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n+1-k), where k = [
 *          (n+1)/2 ], s = (composite numbers).
 * https://oeis.org/A024588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} s(i)·s(n+1-i), where s is the
 *            sequence of composite numbers starting 4,6,8,9,10,
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers, types are supplied by the build. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* External typedefs.  Avoid redefining them. */
/* typedef uint64_t idx_t;          -- supplied by the build */
/* typedef <integer type> seq_t;   -- supplied by the build */

/* ------------------------------------------------------------ */
/*  Simple dynamic sieve that keeps the list of composites.    */
/* ------------------------------------------------------------ */

static bool *sieve = NULL;          /* sieve array: true == marked as composite */
static idx_t sieve_limit = 0;       /* upper bound of current sieve */

/* Array of all composites discovered so far, in order. */
static idx_t *composites = NULL;
static size_t composite_cnt = 0;    /* number of composites stored */

/* ------------------------------------------------------------------
   Ensure that we have at least `need` composite numbers.
   The function keeps an expanding sieve and an array of composites.
   ------------------------------------------------------------------ */
static void build_composites_up_to(size_t need)
{
    if (need <= composite_cnt)
        return;                       /* already have enough */

    /* 1. Expand sieve if necessary. */
    idx_t new_limit = (idx_t)(need * 2 + 20);  /* generous over‑approximation */
    if (new_limit <= sieve_limit)
        new_limit = sieve_limit * 2 + 20;      /* grow further if needed */

    /* Allocate or reallocate the sieve array. */
    bool *new_sieve = (bool *)realloc(sieve, (size_t)(new_limit + 1) * sizeof(bool));
    if (!new_sieve)
        abort();
    sieve = new_sieve;

    /* 2. Initialise new part of the sieve. */
    if (sieve_limit < new_limit) {
        for (idx_t i = (sieve_limit + 1); i <= new_limit; ++i)
            sieve[i] = true;            /* assume prime until proven composite */
    }

    /* 3. Run the Sieve of Eratosthenes from start up to new_limit. */
    for (idx_t p = 2; p * p <= new_limit; ++p) {
        if (sieve[p]) {
            for (idx_t m = p * p; m <= new_limit; m += p)
                sieve[m] = false;       /* m is composite */
        }
    }

    /* 4. Count and collect new composites beyond the previous limit. */
    size_t old_cnt = composite_cnt;
    for (idx_t i = (sieve_limit + 1); i <= new_limit; ++i) {
        if (i >= 4 && !sieve[i]) {      /* keep only composites ≥4 */
            ++composite_cnt;
        }
    }

    /* 5. Resize the composites array if needed. */
    if (composite_cnt > old_cnt) {
        idx_t *new_comps = (idx_t *)realloc(composites, composite_cnt * sizeof(idx_t));
        if (!new_comps)
            abort();
        composites = new_comps;
    }

    /* 6. Fill the composites array. */
    size_t idx = 0;
    for (idx_t i = 4; i <= new_limit; ++i) {
        if (!sieve[i]) {
            composites[idx++] = i;
        }
    }

    sieve_limit = new_limit;
}

/* ------------------------------------------------------------ */
/*  Sequence function.  External linkage as required.          */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1 */

    /* Ensure we have at least n composite numbers. */
    build_composites_up_to((size_t)n);

    /* Compute the dot‑product sum. */
    seq_t result = 0;
    idx_t k = (n + 1) / 2;        /* floor((n+1)/2) */

    for (idx_t i = 1; i <= k; ++i) {
        seq_t c1 = (seq_t)composites[i - 1];
        seq_t c2 = (seq_t)composites[n - i];   /* zero‑based indices */
        result += c1 * c2;
    }

    return result;
}
