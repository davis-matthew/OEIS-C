/*
 * A085519  One of the three possible weight distributions for a [16,7,6]
 *          binary code.
 * https://oeis.org/A085519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0) = 1, a(3) = 44, a(4) = 45, a(5) = 28, a(6) = 10, otherwise
 *            a(n) = 0
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External function that returns the n‑th term of sequence A085519 */
seq_t a(idx_t n)
{
    switch (n)
    {
        case 0: return (seq_t)1;
        case 3: return (seq_t)44;
        case 4: return (seq_t)45;
        case 5: return (seq_t)28;
        case 6: return (seq_t)10;
        default: return (seq_t)0;
    }
}
