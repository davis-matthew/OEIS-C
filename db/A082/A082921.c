/*
 * A082921  Sort the digits of these squares into descending order and drop
 *          zeros to get primes.
 * https://oeis.org/A082921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th perfect square whose digits, when reordered in
 *            descending order and with all zeros removed, form a prime number
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_frequencies, isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_frequencies.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  next_k = 1;           /* next integer to test */

    while (len < n) {
        seq_t m = (seq_t)next_k * (seq_t)next_k;  /* square */

        unsigned int freq[10];
        digit_frequencies(m, freq);              /* count digits 0–9 */

        seq_t r = 0;
        for (int d = 9; d >= 1; --d) {           /* build digits descending, skip 0 */
            for (unsigned int c = 0; c < freq[d]; ++c)
                r = r * 10 + d;
        }

        if (isPrimeGeneric(r)) {                 /* keep only qualifying squares */
            if (len + 1 > cap) {                 /* grow the cache */
                size_t new_cap = cap ? cap * 2 : 1;
                cache = seq_realloc(cache, cap, new_cap);
                cap = new_cap;
            }
            cache[len++] = m;
        }

        ++next_k;
    }

    return cache[n-1];
}
