/*
 * A089929  Algebraic degree of cot(Pi/n).
 * https://oeis.org/A089929
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = phi(n/gcd(n,2))
 * Range:     u8 n <= 256, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses only integers of type uint64_t, which is
   sufficient because n up to the limits of the tests (≈10 000)
   yields φ(m) ≤ m ≤ 10 000.  Thus the result fits in any
   seq_t that can hold ≤ 10 000.  The function is fully
   deterministic and independent of call order. */

#include <stdint.h>
#include <stdbool.h>
/* Make sure the compiler sees the correct external types. */

seq_t a(idx_t n)
{
    /* The OEIS offset is 2; values for n < 2 are not part of the
       sequence.  Returning 0 for them is harmless. */
    if (n < 2)
        return 0;

    /* gcd(n,2) is 1 if n is odd, 2 if n is even.  This can be
       obtained without any helper function. */
    uint64_t g = (n & 1) ? 1 : 2;
    uint64_t m = n / g;         /* n/gcd(n,2) */

    /* Compute φ(m) by factorising m.  This is O(√m) and very fast
       for m ≤ 10 000. */
    uint64_t phi = m;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0)
                t /= p;
            phi -= phi / p;
        }
    }
    if (t > 1)             /* a remaining prime factor > √m */
        phi -= phi / t;

    return (seq_t)phi;
}
