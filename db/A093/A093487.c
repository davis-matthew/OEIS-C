/*
 * A093487  Smallest prime that begins with the digit reversal of prime(n).
 * https://oeis.org/A093487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest prime that begins with rev(prime(n)) (rev is the
 *            decimal digit reversal of an integer)
 * Range:     u8 n <= 7, u16 n <= 32, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   build_primes_upto, isPrimeGeneric, pow10, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/rev.h"
#include "../utils/pow10.h"
#include "../utils/build_primes_upto.h"

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Prime table (static cache).  Limit chosen to provide >10 000 primes. */
    static uint64_t *prime_list = NULL;
    static size_t    prime_count = 0;
    if (!prime_list) {
        /* 2 000 000 contains 148 933 primes > 10 000. */
        const idx_t LIMIT = 2000000;
        build_primes_upto(LIMIT, &prime_list, &prime_count);
    }

    if (n == 0)  /* offset is 1 – any value is fine for n<1. */
        return (seq_t)0;

    /* Get the n‑th prime.  prime_list is 0‑based. */
    uint64_t prime_n = prime_list[(size_t)(n - 1)];

    /* Reversal of the prime gives the desired prefix. */
    seq_t prefix = rev(prime_n);

    /* If the prefix itself is prime, it is the answer. */
    if (isPrimeGeneric(prefix))
        return prefix;

    /* Search for the smallest prime starting with the decimal prefix. */
    for (idx_t suffix_len = 1; ; ++suffix_len) {
        seq_t pow10v = pow10(suffix_len);                  /* 10^suffix_len */

        seq_t low  = prefix * pow10v;                     /* first candidate */
        seq_t high = (prefix + (seq_t)1) * pow10v - (seq_t)1; /* last candidate */

        /* Start at the first odd number ≥ low.  Handle 2 separately. */
        seq_t cand = low;
        if (cand <= (seq_t)2) {
            if (cand == (seq_t)2)
                return (seq_t)2;
            cand = (seq_t)3;
        }
        if ((cand & (seq_t)1) == (seq_t)0)
            cand += (seq_t)1;   /* make it odd */

        for (; cand <= high; cand += (seq_t)2) {
            if (isPrimeGeneric(cand))
                return cand;
        }
        /* No prime found with this suffix length; increase the length. */
    }
}
