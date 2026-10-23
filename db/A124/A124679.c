/*
 * A124679  a(n) = number of conjugacy classes in PSL_3(prime(n)).
 * https://oeis.org/A124679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = p^2 + p if p ≡ 2 (mod 3) or p = 3, * a(n) = (p^2 + p +
 *            10)/3 if p ≡ 1 (mod 3), * where p = prime(n). * The sequence
 *            starts with a(1)=6, a(2)=12, …
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Obtain the nth prime.  get_prime(1) = 2. */
    uint32_t p = get_prime(n);

    /* Compute p^2 in seq_t, taking care to cast early. */
    seq_t pp = (seq_t)p * (seq_t)p;

    if ((p % 3u) == 1u) {
        /* (p^2 + p + 10) / 3  when p ≡ 1 (mod 3). */
        pp = (pp + (seq_t)p + (seq_t)10) / (seq_t)3;
    } else {
        /* p^2 + p  for p ≡ 0 or 2 (mod 3). */
        pp = pp + (seq_t)p;
    }

    return pp;
}
