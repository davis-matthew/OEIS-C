/*
 * A141340  Positive integers n such that A061358(n) = #{primes p | n/2 <= p <
 *          n-1}.
 * https://oeis.org/A141340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, fini, full
 * Formula:   a(n) = nth term of sequence A141340: positive integers n such
 *            that for every prime p with n/2 ≤ p < n-1 the number n-p is also
 *            prime. The maximal term of this sequence is 210, so all terms
 *            are ≤210. The function computes the list up to the maximal term
 *            once and then returns the requested term
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static data that is built on first call.  */
    static bool built = false;
    static seq_t *seq = NULL;      /* array of sequence values */
    static size_t seq_len = 0;     /* number of stored values */

    if (!built) {
        built = true;

        /* Maximum possible value of a term (known property).  */
        const idx_t MAX_TERM = 210;

        /* Sieve of Eratosthenes up to MAX_TERM.  */
        bool *is_prime = (bool *)calloc((size_t)(MAX_TERM + 1), sizeof(bool));
        if (!is_prime) return 0;    /* allocation failure → 0 */

        for (idx_t i = 2; i <= MAX_TERM; ++i) is_prime[i] = true;
        for (idx_t i = 2; i * i <= MAX_TERM; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= MAX_TERM; j += i)
                    is_prime[j] = false;
            }
        }

        /* Generate all terms of the sequence up to MAX_TERM.  */
        size_t cap = 0;
        for (idx_t val = 1; val <= MAX_TERM; ++val) {
            idx_t start = (val + 1) / 2;      /* ceil(val/2) */
            bool ok = true;

            /* Check the defining property.  */
            for (idx_t p = start; p < val - 1; ++p) {
                if (is_prime[p]) {
                    idx_t q = val - p;
                    if (!is_prime[q]) { ok = false; break; }
                }
            }

            if (ok) {
                if (seq_len >= cap) {
                    cap = (cap == 0) ? 16 : cap * 2;
                    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)val;
            }
        }
        free(is_prime);
    }

    /* Return the requested term, or 0 if it does not exist.  */
    if (n == 0) return 0;
    if (n - 1 < seq_len) return seq[n - 1];
    return 0;
}
