/*
 * A057264  Numbers n such that n | 11^n + 10^n + 9^n + 8^n + 7^n + 6^n + 5^n.
 * https://oeis.org/A057264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special sequence: a(n) = the n‑th integer k such that
   k | 11^k + 10^k + 9^k + 8^k + 7^k + 6^k + 5^k. */

#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *terms = NULL;
    static idx_t cap = 0;
    static idx_t len = 0;
    static idx_t last_checked = 0;

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        ++last_checked;
        seq_t mod   = (seq_t)last_checked;
        seq_t sum   = 0;

        /* Compute each power modulo 'last_checked' and accumulate modulo 'mod'. */
        sum = (sum + mod_pow((seq_t)5,  last_checked, mod)) % mod;
        sum = (sum + mod_pow((seq_t)6,  last_checked, mod)) % mod;
        sum = (sum + mod_pow((seq_t)7,  last_checked, mod)) % mod;
        sum = (sum + mod_pow((seq_t)8,  last_checked, mod)) % mod;
        sum = (sum + mod_pow((seq_t)9,  last_checked, mod)) % mod;
        sum = (sum + mod_pow((seq_t)10, last_checked, mod)) % mod;
        sum = (sum + mod_pow((seq_t)11, last_checked, mod)) % mod;

        /* If the sum is a multiple of 'last_checked', store the term. */
        if (sum == 0) {
            if (len == cap) {
                cap = cap ? (cap << 1) : 8;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = (seq_t)last_checked;
        }
    }
    return terms[n - 1];
}
