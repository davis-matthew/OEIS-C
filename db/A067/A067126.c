/*
 * A067126  Numbers for which phi(n) >= phi(k) for all k = 1 to n-1.
 * https://oeis.org/A067126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th integer whose Euler totient φ(n) is a record *
 *            (≥ all previous φ(k), 1 ≤ k < n). The sequence coincides with
 *            primes * after the term 9, with the following exception: *
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=7, a(7)=9, and for
 *            n ≥ 8 * a(n) = the (n‑3)‑rd prime
 * Range:     u8 n <= 57, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the first few terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)5;
    if (n == 6) return (seq_t)7;
    if (n == 7) return (seq_t)9;

    /* For n ≥ 8, the sequence consists of primes:
     * a(n) = prime(n-3)   (1‑based indexing of primes) */
    return get_prime(n - 3);
}
