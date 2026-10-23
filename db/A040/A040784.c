/*
 * A040784  Continued fraction for sqrt(813).
 * https://oeis.org/A040784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued fraction for sqrt(813)
 *  a(0) = 28
 *  a(n) for n>=1 repeats the pattern: 1,1,18,1,1,56
 *  (period 6; a(1)=1, a(2)=1, a(3)=18, a(4)=1, a(5)=1, a(6)=56)
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;

    /* compute (n-1) mod 6 */
    idx_t k = (n - 1) % 6U;  /* 6 is small, fits in idx_t */

    switch (k) {
        case 0: /* a(1) */
        case 1: /* a(2) */
        case 3: /* a(4) */
        case 4: /* a(5) */
            return (seq_t)1;
        case 2: /* a(3) */
            return (seq_t)18;
        case 5: /* a(6) */
            return (seq_t)56;
        default:
            /* Should never happen, but return 0 to avoid undefined behavior */
            return (seq_t)0;
    }
}
