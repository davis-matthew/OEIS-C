/*
 * A018353  Divisors of 245.
 * https://oeis.org/A018353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 245: a(n) = 5^e * 7^f with e∈{0,1}, f∈{0,1,2}, listed in
   ascending order (a(1)=1, a(2)=5, a(3)=7, a(4)=35, a(5)=49, a(6)=245). */

seq_t a(idx_t n)
{
    /* Off‑by‑one: n starts at 1.  For n outside 1..6 return 0. */
    if (n < 1 || n > 6)
        return (seq_t)0;

    /* Determine e (0 or 1) and f (0,1,2) from the index.
       The enumeration is: for f=0..2, for e=0..1. */
    unsigned int zero = (unsigned int)(n - 1);
    unsigned int e = zero & 1U;      /* least significant bit */
    unsigned int f = zero >> 1U;     /* remaining bits */

    /* Small tables for 5^e and 7^f. */
    static const seq_t pow5[2] = { (seq_t)1, (seq_t)5 };
    static const seq_t pow7[3] = { (seq_t)1, (seq_t)7, (seq_t)49 };

    return pow5[e] * pow7[f];
}
