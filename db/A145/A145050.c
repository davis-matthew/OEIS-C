/*
 * A145050  Primes p of the form 4*k+1 for which s=26 is the least positive
 *          integer such that s*p-(floor(sqrt(s*p)))^2 is a square.
 * https://oeis.org/A145050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p ≡ 1 (mod 4) for which s = 26 is the *
 *            smallest positive integer such that * s·p – floor(√(s·p))² * is
 *            a perfect square, and no smaller positive s yields a square. *
 *            The function searches primes sequentially, testing the condition
 *            for * each candidate, and memoises the fou...
 * Range:     u8 none, u16 none, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   isPrimeGeneric, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */
#include "../utils/isqrt_seq.h"       /* seq_t isqrt_seq(seq_t x); */

seq_t a(idx_t n)
{
    static seq_t *found = NULL;   /* array of found primes */
    static size_t len   = 0;      /* number of primes stored   */
    static size_t cap   = 0;      /* current allocation cap   */
    static seq_t cand   = 5;      /* next odd candidate       */
    static bool init    = false;  /* one‑time allocation flag */

    /* One‑time allocation of the dynamic array */
    if (!init) {
        cap   = 16;
        found = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!found) abort();
        init = true;
    }

    /* Generate more primes until we have at least n of them */
    while (len < n) {
        /* Find the next prime that is ≡ 1 (mod 4) */
        while (!((cand & 3u) == 1u && isPrimeGeneric(cand)))
            cand += 2u;               /* stay odd */

        seq_t p = cand;                /* candidate prime */

        /* Check that 1 ≤ s ≤ 25 do NOT produce a square */
        bool ok = true;
        for (idx_t s = 1; s <= 25; ++s) {
            seq_t prod  = p * (seq_t)s;
            seq_t r1    = isqrt_seq(prod);
            seq_t d     = prod - r1 * r1;
            seq_t r2    = isqrt_seq(d);
            if (r2 * r2 == d) {      /* d is a perfect square */
                ok = false;
                break;
            }
        }

        if (ok) {
            /* Verify that s = 26 indeed yields a square */
            seq_t prod  = p * (seq_t)26;
            seq_t r1    = isqrt_seq(prod);
            seq_t d     = prod - r1 * r1;
            seq_t r2    = isqrt_seq(d);
            if (r2 * r2 == d) {
                /* Store the prime in the array */
                if (len == cap) {
                    size_t new_cap = cap << 1;
                    seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    found = tmp;
                    cap   = new_cap;
                }
                found[len++] = p;
            }
        }

        cand += 2u;                    /* next odd number to test */
    }

    return found[n - 1];
}
