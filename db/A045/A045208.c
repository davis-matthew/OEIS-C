/*
 * A045208  Numbers whose base-5 representation contains exactly one 0 and two
 *          4's.
 * https://oeis.org/A045208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑5 representation *
 *            contains exactly one digit 0 and exactly two digits 4
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of found terms */
    static size_t cap   = 0;        /* capacity of array */
    static idx_t next   = 1;        /* next integer to test */
    static size_t sz    = 0;        /* number of terms cached */

    if (!cache) {
        cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }

    while (sz < n) {
        /* test integer 'next' */
        idx_t t = next;
        unsigned zeros = 0, fours = 0;
        while (t > 0) {
            uint8_t d = t % 5;
            if (d == 0) ++zeros;
            else if (d == 4) ++fours;
            t /= 5;
        }
        if (zeros == 1 && fours == 2) {
            if (sz >= cap) {          /* grow array if needed */
                cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
            }
            cache[sz++] = (seq_t)next;
        }
        ++next;
    }
    return cache[n - 1];
}
