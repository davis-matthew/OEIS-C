/*
 * A163998  Primes p having the same parity as the number of partitions of p.
 * https://oeis.org/A163998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A163998: returns the n-th prime p for which the parity of p
   equals the parity of the partition number p(p).  The value is found by
   an incremental search of integers, computing the parity of partition
   numbers via the pentagonal‑number recurrence and testing primality with
   trial division by previously found primes.  The implementation memoises
   results so calls may come in any order. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Local helpers ---------------------------------------------------- */

/* Integer square root for uint64_t (no floating point). */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t x = 0;
    for (int i = 31; i >= 0; --i) {
        idx_t bit = 1ULL << (2 * i);
        if (x + bit <= n) {
            n -= x + bit;
            x += 1ULL << i;
        }
    }
    return x;
}

/* --- Main function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Memoised state shared across calls. */
    static seq_t *primes = NULL;      /* all primes found so far */
    static size_t primes_len = 0;
    static size_t primes_cap = 0;

    static seq_t *qualifies = NULL;   /* primes satisfying parity test */
    static size_t qual_len = 0;
    static size_t qual_cap = 0;

    static unsigned char *parity = NULL;   /* parity of partition numbers */
    static size_t parity_len = 0;
    static size_t parity_cap = 0;

    static idx_t cur = 2;                 /* next integer to test */

    /* Ensure the required number of qualifying primes is available. */
    while (qual_len < (size_t)n) {
        /* Allocate space for parity up to 'cur' if needed. */
        if (parity_cap <= (size_t)cur) {
            size_t new_cap = parity_cap ? parity_cap * 2 : 16;
            while (new_cap <= (size_t)cur) new_cap <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(parity, new_cap);
            if (!tmp) abort();
            /* Zero the new part of the array. */
            for (size_t i = parity_cap; i < new_cap; ++i) tmp[i] = 0;
            parity = tmp;
            parity_cap = new_cap;
        }

        /* Initialise partition parity table if empty. */
        if (parity_len == 0) {           /* p(0) = 1 (odd) */
            parity[0] = 1;
            parity_len = 1;
        }

        /* Compute partition parities up to the current integer. */
        for (idx_t m = parity_len; m <= cur; ++m) {
            unsigned char val = 0;
            for (idx_t k = 1; ; ++k) {
                idx_t p1 = k * (3 * k - 1) / 2;
                if (p1 > m) break;
                val ^= parity[m - p1];
                idx_t p2 = k * (3 * k + 1) / 2;
                if (p2 <= m) val ^= parity[m - p2];
            }
            parity[m] = val;
        }
        parity_len = (size_t)(cur + 1);

        /* Test primality of 'cur' using stored primes. */
        bool is_prime = true;
        idx_t sqrt_cur = _isqrt_u64(cur);
        for (size_t i = 0; i < primes_len && (idx_t)primes[i] <= sqrt_cur; ++i) {
            if (cur % (idx_t)primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Append to the list of all primes. */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = (seq_t)cur;

            /* If parity matches, store this prime as a qualifying term. */
            if (parity[(size_t)cur] == ((cur & 1) ? 1 : 0)) {
                if (qual_len == qual_cap) {
                    size_t new_cap = qual_cap ? qual_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(qualifies, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    qualifies = tmp;
                    qual_cap = new_cap;
                }
                qualifies[qual_len++] = (seq_t)cur;
            }
        }

        ++cur;  /* proceed to next integer */
    }

    return qualifies[n - 1];
}
