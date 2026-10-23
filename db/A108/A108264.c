/*
 * A108264  Consider primes p and q such that p = 2^k + 9 and q = 2^(k+1) + 9
 *          for some k; sequence gives values of p.
 * https://oeis.org/A108264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   isprime_big, pow2, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108264: primes p = 2^k + 9 such that q = 2^(k+1) + 9 is also prime. */

#include "../utils/isprime_big.h"
#include "../utils/pow2.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    /* keep a cache of already computed terms */
    static seq_t *cache   = NULL;
    static idx_t cap      = 0;
    static idx_t sz       = 0;
    static idx_t last_k   = 0; /* last k that was processed */

    if (n == 0) /* undefined, but return 0 harmlessly */
        return (seq_t)0;

    /* generate new terms until we have at least n entries */
    if (n > sz) {
        for (idx_t k = last_k + 1; sz < n; ++k) {
            seq_t p = pow2(k);
            p += (seq_t)9;
            seq_t q = pow2(k + 1);
            q += (seq_t)9;

            if (isprime_big(p) && isprime_big(q))
                push_seq(&cache, &cap, &sz, p);

            last_k = k; /* remember last processed k */
        }
    }

    return cache[n - 1];
}
