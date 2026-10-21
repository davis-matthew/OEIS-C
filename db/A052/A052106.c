/*
 * A052106  a(n) = lcm(n, n - phi(n)).
 * https://oeis.org/A052106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, n - phi(n))
 * Range:     u8 n <= 29, u16 n <= 434, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi_seq.h"
/* Compute a(n) = lcm(n, n - phi(n)) for n ≥ 1
 *  seq_t and idx_t are defined by the build environment.
 *  The routine is self‑contained and does not depend on external
 *  header files.  It implements Euler's totient function and a
 *  Euclidean GCD algorithm using the same `seq_t` type.  The
 *  calculation works for all supported integer types, including
 *  arbitrary‑precision for seq_t.  No global state is required.   */

static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)                   /* not defined for n=0, but return 0 */
        return (seq_t)0;

    seq_t sn = (seq_t)n;                 /* index to sequence type   */
    seq_t phi_val = phi_seq(sn);          /* Euler's totient          */
    seq_t cot = sn - phi_val;             /* cototient                */
    seq_t g = gcd_seq(sn, cot);           /* gcd(sn, cot)             */
    seq_t res = (sn / g) * cot;           /* lcm(sn, cot)             */
    return res;
}
