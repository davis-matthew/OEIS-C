/*
 * A162733  Sum of remainders of the n-th composite mod k, for k=1,2,3,...,n.
 * https://oeis.org/A162733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^n (c_n mod k) where c_n is the nth composite
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Dynamic storage for primes and composites.                       */
/* ------------------------------------------------------------------ */
static seq_t *composites = NULL;      /* array of nth composites (seq_t) */
static size_t composite_count = 0;    /* number of composites stored */
static size_t composite_cap   = 0;    /* allocated capacity */

static uint64_t *primes = NULL;       /* array of primes found (uint64_t)  */
static size_t prime_count    = 0;     /* number of primes stored          */
static size_t prime_cap      = 0;     /* allocated capacity               */

static uint64_t last_checked = 1;     /* last integer examined (for primes) */

/* ------------------------------------------------------------------ */
/*  Reallocation helpers                                                */
/* ------------------------------------------------------------------ */
static void grow_composites(size_t needed) {
    if (composite_cap >= needed) return;
    size_t new_cap = composite_cap ? composite_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
    composites = tmp;
    composite_cap = new_cap;
}

static void grow_primes(size_t needed) {
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    primes = tmp;
    prime_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Generate composites until we have at least n of them.             */
/* ------------------------------------------------------------------ */
static void ensure_composites(idx_t n) {
    while (composite_count < n) {
        uint64_t cand = ++last_checked;        /* next integer to test */

        /* primality test using trial division by known primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store new prime */
            grow_primes(prime_count + 1);
            primes[prime_count++] = cand;
        } else {
            /* store composite */
            grow_composites(composite_count + 1);
            composites[composite_count++] = (seq_t)cand;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Main function implementing the sequence.                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* safety for zero index (unused) */

    ensure_composites(n);               /* guarantees composite exists */

    seq_t c = composites[n - 1];        /* nth composite, 1‑based */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += c % (seq_t)k;
    }
    return sum;
}
