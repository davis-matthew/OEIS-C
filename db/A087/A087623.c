/*
 * A087623  Square array A(n,k) = the cardinality of the set {x in [1,k-1] :
 *          gcd(x,k)=n}, read by rising antidiagonals.
 * https://oeis.org/A087623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9961, u16 n <= 9961, u32 n <= 9961, u64 n <= 9961,
 *            u128 n <= 9961, big any n
 * Helpers:   find_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A087623 : Flattened square array of gcd counts.
 *  For the n-th index, let (m,k) be the pair obtained by reading the array
 *  A(m,k)=|{x∈[1,k‑1]:gcd(x,k)=m}| by rising antidiagonals.
 *  Then a(n) = A(m,k) = 
 *      0                        if k % m ≠ 0 or k = m
 *      EulerPhi(k/m)            otherwise
 *
 *  The mapping from index n to (m,k) is defined as follows:
 *      Let s be the smallest integer with s*(s-1)/2 ≥ n.
 *      let start = ((s-2)*(s-1))/2 + 1.
 *      let i = n - start;            // offset along the s‑th antidiagonal
 *      m = s-1-i;                    // row number
 *      k = i+1;                      // column number
 *
 *  Complexity : O(√k + sqrt(k)) for each call, which is easily < 2 s
 *  for n up to 10 000.  Only standard integer arithmetic is used.
 */

#include <stdint.h>
#include "../utils/find_s.h"

/* Independent Euler phi for unsigned 64‑bit numbers.
 *  Works for all k ≤ 2^64-1 and returns a value that fits in seq_t. */
static seq_t phi_uint64(uint64_t n)
{
    seq_t result = (seq_t)n;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1) {
        result -= result / (seq_t)temp;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Determine (m,k) corresponding to index n. */
    idx_t s = find_s(n);                          /* s≥2 */
    unsigned __int128 prev = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t start = (idx_t)(prev + 1);
    idx_t i = n - start;
    idx_t m = s - 1 - i;
    idx_t k = i + 1;

    if (k % m != 0) return 0;
    if (k == m)     return 0;

    uint64_t div = k / m;                      /* div ≥ 2 */
    return phi_uint64(div);
}
