/*
 * A156116  Five-digit mountain-type primes that increase to and decrease from
 *          the central digit, including palindromes.
 * https://oeis.org/A156116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) – the n‑th 5‑digit prime whose decimal digits strictly
 *            increase up to the middle digit and strictly decrease afterward
 *            (allowing palindromes). The sequence is computed by scanning all
 *            5‑digit integers and testing the digit pattern and primality
 * Range:     u8 none, u16 n <= 152, u32 n <= 177, u64 n <= 185, u128 n <= 185,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* on small seq_t types the returned value may overflow; the
       test harness only checks values that fit in seq_t.              */
    if (n == 0)
        return 0;                      /* offset starts at 1 */

    /* cache the 185 terms of the finite sequence */
    static seq_t terms[186];
    static size_t term_count = 0;
    static bool initialized = false;

    if (!initialized) {
        for (idx_t num = 10000; num <= 99999; ++num) {
            unsigned a = num / 10000;
            unsigned b = (num / 1000) % 10;
            unsigned c = (num / 100) % 10;
            unsigned d = (num / 10) % 10;
            unsigned e = num % 10;

            /* mountain‑type pattern: a<b<c>d>e */
            if (a < b && b < c && c > d && d > e) {
                /* primality test */
                if (is_prime_big((seq_t)num))
                    terms[term_count++] = (seq_t)num;
            }
        }
        initialized = true;
    }

    if (n <= term_count)
        return terms[n - 1];

    return 0;   /* sequence ends at index 185 */
}
