/*
 * A191612  Image of A008578 (the noncomposite numbers) under the "forming"
 *          transformation.
 * https://oeis.org/A191612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A191612
    The sequence is the forming transformation of the noncomposite
    numbers A008578.  For n>3 it can be expressed using only the
    two consecutive values of A008578:

        p = A008578(n‑1)      // the (n‑1)‑th non‑composite number
        q = A008578(n)        // the n‑th   non‑composite number
        d = q – p
        a(n) = ((p / d) + 1) * d

    The first three terms are 1, 2, 3.  The algorithm below
    generates the required primes on demand, avoiding any
    dynamic allocation of the arbitrary‑precision type `seq_t`.
*/

#include <stdint.h>
#include <stddef.h>

/* ----------  Prime generator (using uint64_t for elementary work)  ---------- */

#define PRIME_CAP   200000          /* enough for n up to ~8×10⁴ */

static uint64_t primes[PRIME_CAP];
static size_t primes_cnt = 0;       /* number of primes currently stored */
static uint64_t next_candidate = 2; /* next integer to test for primeness */

/* Test whether x is prime using previously generated primes. */
static int is_prime_uint64(uint64_t x)
{
    if (x <= 1) return 0;
    if (x <= 3) return 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break;          /* p*p > x */
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Grow the prime table until it contains at least (idx+1) primes
   (zero‑based index). */
static void ensure_primes_up_to_idx(size_t idx)
{
    while (primes_cnt <= idx) {
        /* Find next prime starting from next_candidate */
        while (!is_prime_uint64(next_candidate))
            ++next_candidate;

        if (primes_cnt >= PRIME_CAP) {
            /* Too many primes needed – abort as we cannot support */
            abort();
        }

        primes[primes_cnt++] = next_candidate;
        ++next_candidate;
    }
}

/* Return the n‑th non‑composite number (1‑based index). */
static seq_t nth_noncomposite(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                   /* the only non‑composite < 2 */

    /* For n>1, the values are the primes in order. */
    size_t prime_index = (size_t)(n - 2);   /* zero‑based prime index */
    ensure_primes_up_to_idx(prime_index);

    return (seq_t)primes[prime_index];
}

/* ----------  Sequence implementation  ---------- */
seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)n;   /* a(1)=1, a(2)=2, a(3)=3 */

    /* Compute the two consecutive non‑composite numbers. */
    seq_t p = nth_noncomposite(n - 1);
    seq_t q = nth_noncomposite(n);
    seq_t d = q - p;

    /* The required multiple of d that lies in (p, q]. */
    seq_t m = ((p / d) + 1) * d;
    return m;
}
