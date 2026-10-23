/*
 * A125283  Primes p for which there exists no integer n with 1 < n^2 < p such
 *          that n^4 - n^2 + p is prime (i.e., primes that don't arise as q's
 *          in A126769).
 * https://oeis.org/A125283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the nth prime p such that for every integer n>1 with *
 *            n^2 < p, the number n^4 − n^2 + p is composite (i.e., no such n
 *            exists)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/_floor_isqrt_u64.h"

/* Computes the n‑th element (1‑based) of OEIS sequence A125283. */
seq_t a(idx_t n)
{
    static seq_t *seq     = NULL;   /* stored sequence values               */
    static size_t seq_len = 0;      /* number of stored values             */
    static size_t seq_cap = 0;      /* current array capacity              */
    static idx_t  last_prime = 1;   /* last prime examined                 */

    /* Initialise storage on first call. */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0; /* allocation failure – return 0 as a safe fallback */
    }

    /* Generate primes until we have at least n terms in the sequence. */
    while (seq_len < n) {
        /* Find the next prime > last_prime. */
        idx_t cand = last_prime + 1;
        while (!is_prime(cand)) {
            ++cand;
        }

        /* Test whether cand satisfies the A125283 property. */
        bool ok = true;
        idx_t maxn = _floor_isqrt_u64(cand - 1);  /* floor(sqrt(p-1)) */

        for (idx_t i = 2; i <= maxn && ok; ++i) {
            unsigned __int128 n128 = (unsigned __int128)i;
            unsigned __int128 n2   = n128 * n128;        /* n^2                */
            unsigned __int128 val128 = n2 * n2            /* n^4                */
                                      + (unsigned __int128)cand
                                      - n2;                 /* n^4 - n^2 + p     */
            seq_t val = (seq_t)val128;   /* safe cast: val <= 2^64-1 for p <= 4.3e9 */
            if (is_prime_big(val)) {
                ok = false;  /* found a suitable n → discard cand */
            }
        }

        if (ok) {
            /* Ensure capacity. */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) return 0; /* allocation failure – return 0 */
            }
            seq[seq_len++] = (seq_t)cand;
        }

        last_prime = cand;  /* advance to the prime just processed. */
    }

    return seq[n - 1];
}
