/*
 * A176932  Primes of the form 3^n + 10^n.
 * https://oeis.org/A176932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(e) + 10^(e) where e is the (n‑th) exponent such that
 *            the sum is prime (e starts at 0)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, mul_small, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/mul_small.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* internal cache of computed terms */
    static seq_t *terms = NULL;
    static size_t cap   = 0;     /* capacity of terms array   */
    static size_t sz    = 0;     /* current number of terms   */
    static idx_t last_exp = 0;   /* last exponent processed   */
    static seq_t p3 = 1;         /* 3^last_exp                */
    static seq_t p10 = 1;        /* 10^last_exp               */
    static bool init = false;    /* one‑time initialization   */

    if (!init) {
        /* start with a small capacity */
        cap = 16;
        terms = seq_alloc_new_array(cap);
        sz = 0;
        p3  = 1;
        p10 = 1;
        last_exp = 0;
        init = true;
    }

    while (sz < n) {
        seq_t candidate = p3 + p10;
        if (is_prime_big(candidate)) {
            if (sz == cap) {
                cap <<= 1;
                terms = seq_realloc(terms, cap >> 1, cap);
            }
            terms[sz++] = candidate;
        }
        /* advance to next exponent */
        last_exp++;
        p3  = mul_small(p3,  3);
        p10 = mul_small(p10, 10);
    }
    return terms[n - 1];
}
