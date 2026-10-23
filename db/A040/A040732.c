/*
 * A040732  Continued fraction for sqrt(760).
 * https://oeis.org/A040732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040732: Continued fraction coefficients for sqrt(760) */
/* Periodic sequence: a(0) = 27, a(n+10) = a(n) for n > 0 */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)27;
    }
    /* n > 0 */
    idx_t r = n % 10;  /* 0 … 9 */
    switch (r) {
        case 0:  return (seq_t)54; /* a(10), a(20), …  */
        case 1:  return (seq_t)1;
        case 2:  return (seq_t)1;
        case 3:  return (seq_t)3;
        case 4:  return (seq_t)5;
        case 5:  return (seq_t)1;
        case 6:  return (seq_t)5;
        case 7:  return (seq_t)3;
        case 8:  return (seq_t)1;
        case 9:  return (seq_t)1;
        default: return (seq_t)0;  /* never reached */
    }
}
