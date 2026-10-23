/*
 * A124983  Nonprime numbers == 1 (mod 4) with a unique partition as a sum of
 *          2 squares x^2 + y^2.
 * https://oeis.org/A124983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 843, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A124983: Nonprime numbers == 1 (mod 4) with a unique partition
 * as a sum of 2 squares x^2 + y^2.
 *
 * Computation method:
 *   - Numbers m ≡ 1 (mod 4), m ≥ 1.
 *   - m must be composite (or m = 1).
 *   - In the prime factorization of m,
 *        * every prime ≡ 3 (mod 4) occurs with an even exponent.
 *        * at most one prime ≡ 1 (mod 4) is present and it appears with exponent 1.
 *   The above conditions guarantee exactly one unordered pair
 *   (x, y) of non‑negative integers with x² + y² = m.
 *
 * The function generates successive terms until the requested index
 * is available; the sequence is memoized so calls may come in any order.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Memoized cache of sequence values */
    static seq_t *seq   = NULL;   /* sequence data */
    static size_t seq_cap = 0;    /* capacity of seq */
    static size_t seq_len = 0;    /* number of terms stored */

    /* The first term a(1) is 1.  Subsequent terms are found by scanning
     * numbers congruent to 1 mod 4, skipping primes and those that do
     * not meet the factorization uniqueness condition.  */
    while (seq_len < (size_t)n)
    {
        seq_t candidate;
        /* Determine the number to test next. */
        if (seq_len == 0) {
            /* First term */
            candidate = 1;
        } else {
            /* Start from the next number 1 mod 4 after the last found term. */
            candidate = seq[seq_len - 1];
            /* Step by 4 to stay in the residue class 1 (mod 4). */
            candidate += 4;
            /* Search until a suitable candidate is found. */
            while (true)
            {
                /* Skip primes (except 1). */
                if (candidate > 1 && is_prime_big(candidate))
                {
                    candidate += 4;
                    continue;
                }

                /* Factorization check – all arithmetic is done in uint64_t
                 * because for the tested n the values fit in 64 bits
                 * (the sequence grows slowly).  */
                uint64_t m = (uint64_t)candidate;
                uint64_t remaining = m;
                bool ok  = true;
                bool p1_found = false;   /* flag for a prime ≡ 1 (mod 4) */

                /* Remove possible factor 2 – irrelevant for odd m. */
                while ((remaining & 1ULL) == 0)
                    remaining >>= 1;

                /* Trial division by odd numbers up to sqrt(remaining). */
                for (uint64_t p = 3; p * p <= remaining; p += 2)
                {
                    if (remaining % p == 0)
                    {
                        unsigned e = 0;
                        while (remaining % p == 0)
                        {
                            remaining /= p;
                            e++;
                        }

                        if (p % 4 == 3) {
                            if (e & 1) {   /* odd exponent of 3 mod 4 prime */
                                ok = false;
                                break;
                            }
                        } else { /* p ≡ 1 (mod 4) */
                            if (e > 1) {    /* exponent > 1 not allowed */
                                ok = false;
                                break;
                            }
                            if (p1_found) { /* more than one such prime */
                                ok = false;
                                break;
                            }
                            p1_found = true;   /* exponent is 1 */
                        }
                    }
                }

                if (!ok) {
                    candidate += 4;
                    continue;
                }

                /* Handle remaining prime factor > sqrt(remaining). */
                if (remaining > 1)
                {
                    if (remaining == m) {
                        /* candidate itself is a prime – reject */
                        ok = false;
                    } else {
                        if (remaining % 4 == 3) {
                            ok = false;
                        } else { /* remaining ≡ 1 (mod 4) */
                            if (p1_found) { /* second 1 mod 4 prime */
                                ok = false;
                            }
                            /* e = 1 automatically */
                        }
                    }
                }

                if (ok) break;  /* candidate satisfies all conditions */
                candidate += 4;
            }
        }

        /* Append the found candidate to the cache. */
        if (seq_len == seq_cap)
        {
            /* Grow capacity: start with 16, then double. */
            seq_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = candidate;
    }

    return seq[n - 1];
}
