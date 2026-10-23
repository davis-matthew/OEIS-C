/*
 * A040837  Continued fraction for sqrt(867).
 * https://oeis.org/A040837
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

/* CF for sqrt(867). The partial quotients are periodic with period 6:
   a(0) = 29
   for n > 0, a(n) = period[(n-1) % 6] where period = {2,4,29,4,2,58} */
seq_t a(idx_t n)
{
    /* Handle the first term separately. */
    if (n == 0)
        return (seq_t)29;

    /* Periodic pattern after the initial term. */
    static const seq_t period[6] = {
        (seq_t)2,  /* a(1) */
        (seq_t)4,  /* a(2) */
        (seq_t)29, /* a(3) */
        (seq_t)4,  /* a(4) */
        (seq_t)2,  /* a(5) */
        (seq_t)58  /* a(6) = 2 * 29 */
    };

    /* Compute index into the period using modulo arithmetic. */
    idx_t idx = (n - 1) % 6;
    return period[idx];
}
