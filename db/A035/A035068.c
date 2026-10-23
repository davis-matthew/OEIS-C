/*
 * A035068  Palindromic prime lengths of factorials: see A035067.
 * https://oeis.org/A035068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_palindrome, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromic prime lengths of factorials (OEIS A035068).
 *
 * The sequence is defined as follows:
 *   a(n) = the n‑th digit count of k! for all k ≥ 1 such that
 *          the digit count itself is a palindrome and a prime.
 *
 * This implementation builds the list lazily. It keeps a running
 * sum of log10(k) to compute digit counts quickly, tests the
 * palindrome property with is_palindrome() and the primality
 * with is_prime(), and caches found terms for subsequent calls.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    if (n == 0) abort();   /* offset is 1 */

    /* Static state shared across calls */
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;     /* allocated cache size   */
    static size_t  count   = 0;     /* number of terms cached */
    static idx_t   last_k  = 0;     /* last factorial index processed */
    static long double log_sum = 0.0L;   /* Σ log10(k) for k=1..last_k */

    while (count < n) {
        /* Move to next factorial number */
        last_k++;
        log_sum += log10l((long double)last_k);

        /* Number of decimal digits of last_k! = floor(log10(last_k!)) + 1 */
        unsigned long long d_cnt = (unsigned long long)floorl(log_sum) + 1ULL;
        seq_t d_val = (seq_t)d_cnt;

        /* Check palindromic‑prime property */
        if (is_palindrome(d_val, 10) && is_prime((idx_t)d_cnt)) {
            /* Grow cache if necessary */
            if (count == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = d_val;
        }
    }

    return cache[n - 1];
}
