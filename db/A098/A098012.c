/*
 * A098012  Triangle read by rows in which the k-th term in row n (n >= 1, k =
 *          1..n) is Product_{i=0..k-1} prime(n-i).
 * https://oeis.org/A098012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is read from the triangle whose r‑th row lists partial
 *            products of the first r primes in descending order. For n ≥ 1,
 *            let r be the smallest integer with r(r+1)/2 ≥ n and let k = n –
 *            (r-1)r/2 (1 ≤ k ≤ r). Then a(n) = ∏_{i=0}^{k-1} prime(r-i)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 54, u64 n <= 135,
 *            u128 n <= 374, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Position within the row. */
    idx_t k = n - (r - 1) * r / 2;   /* 1 ≤ k ≤ r */

    seq_t prod = 1;
    for (idx_t i = 0; i < k; ++i) {
        uint32_t p = get_prime(r - i);   /* 1‑based prime index */
        prod *= (seq_t)p;
    }

    return prod;
}
