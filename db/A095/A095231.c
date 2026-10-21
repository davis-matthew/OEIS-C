/*
 * A095231  a(1)=1; a(n)=least positive integer such that the concatenation of
 *          all terms, including a(n), is == 1 (mod n).
 * https://oeis.org/A095231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   digit_len, pow10_mod_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A095231
 *
 * a(1) = 1
 * a(n) = least positive integer such that the concatenation of all terms,
 *        including a(n), is congruent to 1 modulo n.
 *
 * The algorithm builds the sequence incrementally.  For each n we
 * concatenate the previous terms modulo n and then search for the
 * smallest x satisfying (R * 10^len(x) + x) ≡ 1 (mod n).
 *
 * The implementation uses a static cache up to a compile‑time
 * maximum size (2 000 000 terms) so that no dynamic memory allocation
 * is required, making the code valid both in C11 and C++17.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
/* digit_len is provided by the build */
#include "../utils/digit_len.h"
#include "../utils/pow10_mod_simple.h"

#define SEQ_CACHE_MAX 2000000   /* sufficient for all test cases (n ≤ 10 000) */

static seq_t  cache[SEQ_CACHE_MAX];
static int    clen[SEQ_CACHE_MAX];   /* decimal digit length of cache[i] */
static size_t cache_size = 0;        /* number of terms already computed */

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 for safety */

    if ((size_t)n > SEQ_CACHE_MAX)
        return (seq_t)0;          /* out of bounds – undefined behaviour */

    /* generate sequence up to n if needed */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        seq_t mod = (seq_t)(i + 1);   /* current modulus */

        /* compute concatenation R modulo mod of all previous terms */
        seq_t R = (seq_t)0;
        for (size_t j = 0; j < i; ++j) {
            seq_t pow10j = pow10_mod_simple(mod, clen[j]);
            seq_t term   = cache[j] % mod;
            R = (R * pow10j + term) % mod;
        }

        /* find least positive integer x such that
           (R * 10^len(x) + x) % mod == 1 % mod */
        seq_t x = (seq_t)1;
        while (true) {
            int lenx  = digit_len(x);
            seq_t pow10x = pow10_mod_simple(mod, lenx);
            seq_t cand = (R * pow10x + (x % mod)) % mod;
            if (cand == (seq_t)(1 % mod))
                break;
            ++x;
        }

        /* store new term */
        cache[i] = x;
        clen[i]  = digit_len(x);
    }

    cache_size = (size_t)n;   /* all terms up to n are now cached */
    return cache[n - 1];
}
