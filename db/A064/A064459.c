/*
 * A064459  a(n) = Sum_{k>=1} floor(n/12^k).
 * https://oeis.org/A064459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k>=1} floor(n/12^k)
 * Range:     u8 n <= 388, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t div = 12;          /* 12^1 */
    seq_t n_seq = (seq_t)n;

    while (div <= n_seq) {
        result += n_seq / div;            /* floor division */
        /* If multiplying by 12 would overflow or exceed n_seq, stop */
        if (div > n_seq / 12) break;
        div *= 12; /* next power of 12 */
    }

    return result;
}
