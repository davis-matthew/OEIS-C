/*
 * A075522  Number of steps to reach the first nonprime (=A075523(n)) starting
 *          with the n-th prime p and iterating the map k -> 4*k+(p mod 4), or
 *          -1 if no integer is ever reached.
 * https://oeis.org/A075522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reach the first nonprime in the orbit
 *            * starting with the n‑th prime p and iterating the map * k ->
 *            4*k + (p mod 4). The count starts at 0 for the * initial prime
 *            and increments for every application of the map * until the
 *            number is no longer prime. If the orbit never *...
 * Range:     u8 n <= 16, u16 n <= 104, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Fetch the n-th prime (1‑based). */
    seq_t p = get_prime(n);

    /* The constant shift: p mod 4.  Using bitwise & is safe for all
     * integer types because only the 2‑bit pattern of a mod 4 is needed. */
    const seq_t r = p & 3U;

    seq_t k = p;         /* current value in the orbit          */
    seq_t steps = 0;     /* number of applications of the map   */

    /* Loop until k is no longer prime.  The function is_prime_big
     * works for both fixed‑width types and arbitrary‑precision types.
     */
    while (is_prime_big(k))
    {
        ++steps;                    /* we performed one more step     */
        k = 4 * k + r;              /* next value in the orbit        */
    }

    return steps;   /* steps is the required a(n) */
}
