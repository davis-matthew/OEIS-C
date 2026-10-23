/*
 * A115207  a(1)=1. a(n) = number of earlier terms of the sequence which when
 *          added to n produce a composite number.
 * https://oeis.org/A115207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of earlier terms of the sequence which when added
 *            * to n produce a composite number
 * Range:     u8 n <= 294, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static storage for the sequence (1‑indexed). */
static seq_t *a_seq  = NULL;   /* a_seq[0] = a(1)           */
static size_t a_cap = 0;       /* capacity of a_seq          */
static size_t a_len = 0;       /* number of values stored    */

/* Simple prime sieve used for rapid primality testing.  Size up to sieve_cap. */
static unsigned char *prime_sieve = NULL;
static idx_t sieve_cap = 0;

/* Ensure that a_seq has space for at least n elements (n ≥ 1). */
static void
ensure_a_cap(idx_t n)
{
    if ((idx_t)a_cap >= n)
        return;
    size_t new_cap = (a_cap == 0) ? 16 : a_cap;
    while ((idx_t)new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_seq = tmp;
    a_cap = new_cap;
}

/* Build or extend the prime sieve so that it is valid up to at least limit. */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_cap)
        return;

    unsigned char *tmp = (unsigned char *)realloc(prime_sieve, (size_t)(limit + 1));
    if (!tmp)
        abort();
    prime_sieve = tmp;
    sieve_cap   = limit;

    for (idx_t i = 0; i <= limit; ++i)
        prime_sieve[i] = 1;
    if (limit >= 0)
        prime_sieve[0] = 0;
    if (limit >= 1)
        prime_sieve[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (prime_sieve[p])
            for (idx_t m = p * p; m <= limit; m += p)
                prime_sieve[m] = 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined below the offset */

    /* First call: initialise. */
    if (a_len == 0) {
        ensure_a_cap(n);
        a_seq[0] = (seq_t)1;   /* a(1) = 1 */
        a_len   = 1;
        ensure_sieve(2);       /* at least first prime */
    }

    /* Grow storage & sieve if needed. */
    if ((idx_t)a_len < n) {
        ensure_a_cap(n);
        ensure_sieve(2 * n);

        /* Compute missing terms up to n. */
        for (idx_t i = a_len + 1; i <= n; ++i) {
            seq_t cnt = 0;
            for (idx_t j = 1; j < i; ++j) {
                idx_t s = i + (idx_t)a_seq[j - 1];
                if (s >= 2 && !prime_sieve[s])
                    ++cnt;
            }
            a_seq[i - 1] = cnt;
        }
        a_len = (size_t)n;
    }

    return a_seq[n - 1];
}
