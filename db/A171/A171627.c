/*
 * A171627  a(n) = ceiling(n-th composite/n).
 * https://oeis.org/A171627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = ceil( A002808(n) / A000027(n) ) where A002808(n) is the
 *            n-th composite number and A000027(n) = n. For n >= 8 the
 *            sequence satisfies a(n) = A171623(n) + 1. This implementation
 *            only uses ordinary integer arithmetic; no floating‑point is
 *            required
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------- */
/*  Globals (static storage)                                       */
/* --------------------------------------------------------------- */
static seq_t *composites = NULL;      /* array of composites */
static size_t composites_cap  = 0;     /* capacity of composites   */
static size_t composites_used = 0;     /* number of composites stored */

static uint64_t *primes = NULL;       /* array of prime numbers   */
static size_t primes_cap  = 0;        /* capacity of primes      */
static size_t primes_used = 0;        /* number of primes stored  */

static uint64_t current = 3;          /* last integer tested      */

/* --------------------------------------------------------------- */
/*  Helper: grow a uint64_t array to at least 'needed' elements   */
/* --------------------------------------------------------------- */
static void grow_primes(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* --------------------------------------------------------------- */
/*  Helper: grow a seq_t array to at least 'needed' elements      */
/* --------------------------------------------------------------- */
static void grow_composites(size_t needed)
{
    if (composites_cap < needed) {
        size_t new_cap = composites_cap ? composites_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        composites = tmp;
        composites_cap = new_cap;
    }
}

/* --------------------------------------------------------------- */
/*  Ensure that at least 'n' composite numbers have been          */
/*  generated.  The primes array is automatically extended.       */
/* --------------------------------------------------------------- */
static void ensure_composites(idx_t n)
{
    /* Prime list must contain at least 2 and 3. */
    if (primes_used < 2) {
        grow_primes(3);
        primes[0] = 2;
        primes[1] = 3;
        primes_used = 2;
    }

    while (composites_used < n) {
        ++current;               /* candidate number to test */
        if (current < 4) continue; /* first composite is 4 */

        int is_prime = 1;
        for (size_t i = 0; i < primes_used; ++i) {
            uint64_t p = primes[i];
            if (p * p > current) break;
            if (current % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime. */
            grow_primes(primes_used + 1);
            primes[primes_used++] = current;
        } else {
            /* Store the new composite. */
            grow_composites(composites_used + 1);
            composites[composites_used++] = (seq_t)current;
        }
    }
}

/* --------------------------------------------------------------- */
/*  Main sequence function                                       */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the OEIS offset, harmless */

    ensure_composites(n);

    seq_t comp = composites[n - 1];    /* n-th composite (0‑based index) */
    seq_t div  = (seq_t)n;             /* divisor n */

    return (comp + div - 1) / div;     /* ceil(comp / n) */
}
