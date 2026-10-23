/*
 * A178572  Numbers with ordered partitions that have periods of length 5.
 * https://oeis.org/A178572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (25*n - 3) / 2
 * Range:     u8 n <= 4, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   sequence A178572: numbers with ordered partitions that have periods of length 5 */
/*   Offsetting to 1: a(1)=11   (note 10 + j with j=1 from the general formula). */

seq_t a(idx_t n) {
    /* n is at least 1; behaviour below offset is irrelevant. */
    seq_t N = (seq_t)n;                  /* cast to seq_t for arithmetic */
    seq_t prod = (seq_t)25 * N;          /* 25*n, fits for all tested n */
    prod -= (seq_t)3;                    /* 25*n - 3 */

    /* Avoid intermediate overflow by dividing by 2 as early as possible. */
    if (N & 1) {                          /* N odd → prod is even */
        return N * (prod / 2);
    } else {                              /* N even  */
        return (N / 2) * prod;
    }
}
