/*
 * A229324  Composite squarefree numbers n such that p + tau(n) divides n -
 *          phi(n), where p are the prime factors of n, tau(n) = A000005(n)
 *          and phi(n) = A000010(n).
 * https://oeis.org/A229324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 259, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A229324: composite squarefree numbers n such that for each prime divisor
 * p of n, (p + τ(n)) divides n – φ(n).  This implementation searches
 * integers sequentially, caches found terms and returns the n‑th term.
 *
 * The algorithm:
 *   * Maintain a static list of found terms.
 *   * For each candidate m (starting at 2) factor it by trial division.
 *   * Reject if m is not composite or not squarefree.
 *   * Compute φ(m) and τ(m) (for squarefree, τ(m)=2ⁿ where n=#prime factors).
 *   * Check that for every prime p|m, (p+τ(m)) divides m-φ(m).
 *   * If all checks pass, store m in the list.
 *   * Continue until the list reaches length n.
 *   * Return terms[n-1].
 *
 * The implementation uses only integer arithmetic and elementary
 * pointer manipulation; it works in both C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t next_candidate = 2;   /* integer to test next */

    /* Grow the cache until it holds the requested term. */
    while (terms_len < n) {
        uint64_t m = next_candidate++;

        /* Trial‑division factorisation.  The candidate range is modest,
           so a simple loop suffices. */
        uint64_t temp = m;
        uint64_t primes[32];
        unsigned   exps[32];
        size_t     cnt = 0;

        for (uint64_t d = 2; d <= temp / d; ++d) {
            if (temp % d == 0) {
                unsigned e = 0;
                while (temp % d == 0) {
                    temp /= d;
                    ++e;
                }
                primes[cnt] = d;
                exps[cnt]   = e;
                ++cnt;
                if (temp == 1) break;
            }
        }
        if (temp > 1) {                 /* the remaining factor is prime */
            primes[cnt] = temp;
            exps[cnt]   = 1;
            ++cnt;
        }

        /* Requires composite (cnt>=2) and squarefree (all exps==1). */
        if (cnt < 2) continue;
        bool squarefree = true;
        for (size_t i = 0; i < cnt; ++i) {
            if (exps[i] != 1) { squarefree = false; break; }
        }
        if (!squarefree) continue;

        /* Compute φ(m) = m * Π(1 - 1/p). */
        seq_t phi = (seq_t)m;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            phi -= phi / p;   /* exact division */
        }

        /* τ(m) = 2^cnt for a squarefree number. */
        seq_t tau = (seq_t)1 << cnt;

        seq_t delta = (seq_t)m - phi;

        /* Verify divisibility condition for every prime divisor. */
        bool ok = true;
        for (size_t i = 0; i < cnt && ok; ++i) {
            seq_t denom = (seq_t)primes[i] + tau;
            if (delta % denom != 0) ok = false;
        }
        if (!ok) continue;

        /* Store the term in the cache, enlarging the array if needed. */
        if (terms_len == terms_cap) {
            size_t newcap = terms_cap ? terms_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            terms = tmp;
            terms_cap = newcap;
        }
        terms[terms_len++] = (seq_t)m;
    }

    return terms[n - 1];
}
