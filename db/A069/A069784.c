/*
 * A069784  Numbers m such that gcd(d((m!)^3), d(m!)) = 2^k, i.e., is a power
 *          of 2; d = A000005.
 * https://oeis.org/A069784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069784: Numbers m such that gcd(d((m!)^3), d(m!)) is a power of 2.
   Holmes' theorem shows the set is finite: the only solutions are
   m = 1,2,…,9,12,13,14,18,19,30,31,32,35,38,39.
   Therefore a(n) is defined for 1 ≤ n ≤ 20, and a(n)=0 for n > 20. */
seq_t a(idx_t n)
{
    /* sequence values (index 1 → 20) */
    static const seq_t seq[20] = {
        1,  /* a(1) */
        2,  /* a(2) */
        3,  /* a(3) */
        4,  /* a(4) */
        5,  /* a(5) */
        6,  /* a(6) */
        7,  /* a(7) */
        8,  /* a(8) */
        9,  /* a(9) */
        12, /* a(10) */
        13, /* a(11) */
        14, /* a(12) */
        18, /* a(13) */
        19, /* a(14) */
        30, /* a(15) */
        31, /* a(16) */
        32, /* a(17) */
        35, /* a(18) */
        38, /* a(19) */
        39  /* a(20) */
    };

    return (n >= 1 && n <= 20) ? seq[n - 1] : (seq_t)0;
}
