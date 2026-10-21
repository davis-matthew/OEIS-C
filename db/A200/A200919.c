/*
 * A200919  Number of crossings on periodic braids with n strands such that
 *          all strands meet.
 * https://oeis.org/A200919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, nice, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = 0                for n < 4
 *   a(n) = n^2/2 - 3*n + 5           for n even (n ≥ 4)
 *   a(n) = ((n-1)^2)/2 - 3*n + 8
 *            + floor((n+1)^2/16)   for n odd (n ≥ 5)
 */

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n < 4) {
        return (seq_t)0;
    }

    seq_t n_seq = (seq_t)n;
    if ((n_seq & 1ull) == 0) {            /* even n */
        /* a(n) = n^2/2 - 3*n + 5 */
        seq_t n_sq = n_seq * n_seq;       /* n^2 */
        seq_t half_n_sq = n_sq / (seq_t)2;
        seq_t res = half_n_sq - (seq_t)3 * n_seq + (seq_t)5;
        return res;
    } else {                              /* odd n */
        /* a(n) = ((n-1)^2)/2 - 3*n + 8 + floor((n+1)^2/16) */
        seq_t n1 = n_seq - (seq_t)1;
        seq_t n1_sq = n1 * n1;
        seq_t term1 = n1_sq / (seq_t)2 - (seq_t)3 * n_seq + (seq_t)8;

        seq_t n2 = n_seq + (seq_t)1;
        seq_t n2_sq = n2 * n2;
        seq_t term2 = n2_sq / (seq_t)16;   /* floor division */

        return term1 + term2;
    }
}
