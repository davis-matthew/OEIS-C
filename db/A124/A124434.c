/*
 * A124434  LCM (least common multiple) of A001043 (sum of consecutive primes)
 *          and A001223 (difference of consecutive primes).
 * https://oeis.org/A124434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = lcm(prime(n+1)+prime(n), prime(n+1)-prime(n)) * =
 *            (prime(n+1)^2 − prime(n)^2)/2 for n > 1 * a(1) = 5
 * Range:     u8 n <= 14, u16 n <= 428, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

static seq_t lcm_seq(seq_t x, seq_t y)
{
    /* Compute lcm(x,y) = x/gcd(x,y)*y */
    return (x / gcd(x, y)) * y;
}

seq_t a(idx_t n)
{
    /* a(1) is a special case because the closed form would give 2.5 */
    if (n == 1) {
        return (seq_t)5;
    }

    /* Retrieve the nth and (n+1)th primes (sequence is 1‑indexed) */
    seq_t p_n     = (seq_t)get_prime(n);
    seq_t p_next  = (seq_t)get_prime(n + 1);

    /* Sum and difference of consecutive primes */
    seq_t sum  = p_n + p_next;
    seq_t diff = p_next - p_n;

    /* lcm of the sum and difference gives the desired term */
    return lcm_seq(sum, diff);
}
