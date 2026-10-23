/*
 * A162790  a(n) = A162530(n)/10.
 * https://oeis.org/A162790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A162790: primes >10 plus composites 10,12,14,15,16,20,25. */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num = 10;
    while (1) {
        bool include = false;
        switch (num) {
            case 10: case 12: case 14: case 15: case 16: case 20: case 25:
                include = true; break;
            default:
                if (is_prime_small((seq_t)num)) include = true;
        }
        if (include) {
            count++;
            if (count == n) {
                return (seq_t)num;
            }
        }
        num++;
    }
}
