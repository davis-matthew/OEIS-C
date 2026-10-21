/*
 * A094207  a(n) = prime(4n-3) + prime(4n-2) + prime(4n-1) + prime(4n).
 * https://oeis.org/A094207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(4n-3)+prime(4n-2)+prime(4n-1)+prime(4n) (n >= 1)
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"  /* provides uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    if (n == 0)  /* behavior for n < 1 is irrelevant */
        return 0;

    /* base index for the first prime to sum */
    idx_t base = 4 * n - 3;

    seq_t sum = 0;
    sum += (seq_t) get_prime(base);
    sum += (seq_t) get_prime(base + 1);
    sum += (seq_t) get_prime(base + 2);
    sum += (seq_t) get_prime(base + 3);

    return sum;
}
