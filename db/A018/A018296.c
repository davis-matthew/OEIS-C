/*
 * A018296  Divisors of 130.
 * https://oeis.org/A018296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence lists the positive divisors of 130 in increasing order. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Generate the fixed list of 8 divisors of 130: 1,2,5,10,13,26,65,130. */
    static seq_t divs[8];
    const uint64_t primes[3] = { 2ULL, 5ULL, 13ULL };
    const int    exps  [3] = { 1,   1,   1     };

    size_t sz = 1;          /* number of divisors stored so far (initially 1). */
    divs[0] = 1;            /* the divisor 1 */

    for (int pi = 0; pi < 3; ++pi) {
        uint64_t p = primes[pi];
        int exp = exps[pi];
        size_t cur = sz;    /* size before processing this prime factor   */
        for (size_t i = 0; i < cur; ++i) {
            seq_t val = divs[i];
            for (int e = 1; e <= exp; ++e) {
                val *= (seq_t)p;
                divs[sz++] = val;
            }
        }
    }

    /* n is 1‑based; return the (n‑1)‑th divisor if it exists */
    if (n >= 1 && (idx_t)(n - 1) < sz) {
        return divs[(size_t)(n - 1)];
    }
    return 0;   /* undefined for n > 8 */
}
