/*
 * A110416  a(n) = Sum_{k<n, k!==0 (mod n)} k!/n.
 * https://oeis.org/A110416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{1 ≤ k < n, n | k!} k! / n, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 14, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If no k < n satisfies n | k!, the sum is 0.                */

#include <stdint.h>

static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1.   */
    if (n == 1) {
        return 1;
    }

    /* Find the smallest k (< n) such that n divides k!  */
    uint64_t modn = 1;
    idx_t first_k = 0; /* 0 means “none found yet” */
    for (idx_t k = 1; k < n; ++k) {
        modn = (modn * k) % n;
        if (modn == 0) {
            first_k = k;
            break;
        }
    }
    if (first_k == 0) {
        /* n is prime or 4 (or any n not dividing any k! with k < n) */
        return 0;
    }

    /* Compute the first quotient q0 = first_k! / n using gcd reduction,
       to keep the intermediate value within seq_t limits.           */
    uint64_t rem = n;
    seq_t q = 1;
    for (idx_t k = 1; k <= first_k; ++k) {
        uint64_t t = k;
        uint64_t g = gcd_uint64(t, rem);
        t /= g;
        rem /= g;
        q *= (seq_t)t;
    }
    /* After the loop rem should be 1; if not, something went wrong,
       but the definition guarantees it.                           */

    /* Sum over all k from first_k to n-1.                         */
    seq_t sum = 0;
    for (idx_t k = first_k; k < n; ++k) {
        if (k > first_k) {
            q *= (seq_t)k;
        }
        sum += q;
    }
    return sum;
}
