/*
 * A018726  Divisors of 928.
 * https://oeis.org/A018726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 928: a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 8, a(5) = 16,
   a(6) = 29, a(7) = 32, a(8) = 58, a(9) = 116, a(10) = 232,
   a(11) = 464, a(12) = 928; all subsequent terms are 0. */
seq_t a(idx_t n)
{
    static const seq_t DIVS[] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)16,
        (seq_t)29, (seq_t)32, (seq_t)58, (seq_t)116,
        (seq_t)232, (seq_t)464, (seq_t)928
    };
    if (n >= 1 && n <= sizeof(DIVS)/sizeof(DIVS[0]))
        return DIVS[n - 1];
    return (seq_t)0;
}
