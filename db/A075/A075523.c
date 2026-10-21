/*
 * A075523  First nonprime reached when starting with the n-th prime p and
 *          iterating the map k -> 4*k+(p mod 4), or -1 if no integer is ever
 *          reached.
 * https://oeis.org/A075523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A075523 – first non‑prime reached from the n‑th prime using k -> 4k+(p mod 4)
 *  For each n (1‑based), let p be the n‑th prime.  Starting with k = p, repeat
 *  k = 4*k + (p mod 4) until k is composite.  Return that composite.
 *  If the process never terminates, return (seq_t)-1.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* obtain the n‑th prime (n is 1‑based) */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* p mod 4 */
    const seq_t r = p & 3;

    /* iterate the map until a composite is found */
    seq_t k = p;
    while (is_prime_big(k))
    {
        /* k = 4*k + r */
        k = (k << 2) + r;
    }

    /* if the loop never exits (unlikely), return -1 */
    if (k == p) {          /* safeguard against an impossible infinite loop */
        return (seq_t)(-1);
    }

    return k;
}
