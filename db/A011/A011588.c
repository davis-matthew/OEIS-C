/*
 * A011588  Legendre symbol (n,31).
 * https://oeis.org/A011588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   Legendre symbol (n,31). Uses Euler's criterion: (n/31) = n^15
 *            mod 31 ⇒ return 0 if n≡0 (mod 31), 1 if the result is 1,
 *            otherwise -1. This works for any idx_t (uint64_t) and any seq_t
 *            (signed integer or big‑int)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 31.  idx_t is uint64_t so this is safe. */
    uint64_t r = n % 31;
    if (r == 0) return (seq_t)0;

    /* Euler's criterion: r^((p-1)/2) mod p, here p=31, exponent=15. */
    uint64_t res = 1;
    for (int i = 0; i < 15; ++i) {
        res = (res * r) % 31;
    }

    /* Map the result to the Legendre symbol values. */
    return (res == 1) ? (seq_t)1 : (seq_t)-1;
}
