/*
 * A096246  Base-2 deletable primes (written in base 10).
 * https://oeis.org/A096246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Base‑2 deletable primes computed by recursive deletion. 2 and 3
 *            are deletable by convention. A prime p (p > 3) is deletable iff
 *            there exists a digit deletion in its binary representation that
 *            yields a deletable prime (and the deletion may not introduce
 *            leading zeros). The sequence is generated in...
 * Range:     u8 n <= 32, u16 n <= 1833, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_deletable, is_deletable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_deletable.h"
#include "../utils/is_deletable.h"

/* Public function – OEIS sequence A096246 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */

    /* Static state persistent across calls. */
    static bool initialized = false;
    static idx_t *primes = NULL;    /* all primes found so far */
    static size_t primes_sz = 0, primes_cap = 0;
    static idx_t *del = NULL;       /* deletable primes found so far */
    static size_t del_sz = 0, del_cap = 0;
    static idx_t next_cand = 5;     /* next odd number to test for primality */

    /* One‑time initialization. */
    if (!initialized) {
        del_cap = 64;
        primes_cap = 64;
        del = (idx_t *)malloc(del_cap * sizeof(idx_t));
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));

        /* Base cases 2 and 3 are deletable. */
        del[del_sz++] = 2;
        del[del_sz++] = 3;
        primes[primes_sz++] = 2;
        primes[primes_sz++] = 3;

        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (del_sz < n) {
        /* Find next prime candidate. */
        idx_t cand = next_cand;
        while (1) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_sz; ++i) {
                idx_t p = primes[i];
                if (p * p > cand) break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;
            cand += 2;            /* skip even numbers */
        }

        /* Candidate is prime; check deletability. */
        if (cand >= 5) {          /* 2 and 3 are already handled */
            if (is_deletable(cand, del, del_sz)) {
                if (del_sz == del_cap) {
                    del_cap = (del_cap == 0) ? 1 : del_cap << 1;
                    del = (idx_t *)realloc(del, del_cap * sizeof(idx_t));
                }
                del[del_sz++] = cand;
            }
        }

        /* Record the prime for future divisibility checks. */
        if (primes_sz == primes_cap) {
            primes_cap = (primes_cap == 0) ? 1 : primes_cap << 1;
            primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
        }
        primes[primes_sz++] = cand;

        /* Advance to the next candidate. */
        next_cand = cand + 2;
    }

    return (seq_t)del[n - 1];
}
