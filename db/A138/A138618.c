/*
 * A138618  Triangle of exponentials of Mangoldt function M(n) read by rows,
 *          in which row products give the natural numbers.
 * https://oeis.org/A138618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A138618 – Triangle of exponentials of the Mangoldt function
   read by rows.  For a given linear index n (offset 1) this function
   returns the corresponding entry of the triangle.

   Let r be the smallest integer such that r( r + 1) / 2  ≥  n
   (so the nth entry lies in row r).  Let k = n – r(r – 1)/2  (1‑based
   column within that row).  Then

      if r mod k ≠ 0                =>  a(n) = 1
      else let m = r / k
           if m is a prime power m = p^e   => a(n) = p
           otherwise                       => a(n) = 1
*/
#include <stdint.h>

/* small helper: smallest prime factor of a positive integer  */
static uint64_t spf_uint64(uint64_t n)
{
    if (n <= 1) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return i;
    }
    return n;            /* n is prime */
}

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1; /* out of bounds – safe default */

    /* binary search for the smallest r with r(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* compute column k within the row */
    __uint128_t prev_sum = (__uint128_t)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev_sum);   /* 1‑based */

    /* if not a divisor, entry is 1 */
    if (r % k != 0) return (seq_t)1;

    idx_t d = r / k;      /* m = r/k */

    if (d == 1) return (seq_t)1;        /* e^{Λ(1)} = 1 */

    /* check if d is a prime power */
    uint64_t p = spf_uint64(d);
    uint64_t rem = d;
    while (rem % p == 0) rem /= p;
    if (rem == 1)            /* d = p^e */
        return (seq_t)p;
    else
        return (seq_t)1;     /* not a pure prime power */
}
