/*
 * A110030  Number of consecutive integers starting with n needed to sum to a
 *          Niven number.
 * https://oeis.org/A110030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = minimal k such that k*n + k*(k-1)/2 is a Niven number *
 *            (i.e., divisible by its base‑10 digit sum). Implements a
 *            straightforward * search, suitable for n up to at least 10 000
 * Range:     u8 n <= 51, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; undefined for n<1 */
    idx_t k = 1;
    while (1) {
        seq_t kseq = k;
        seq_t S     = kseq * (seq_t)n + kseq * (kseq - 1) / 2;

        /* compute decimal digit sum of S */
        seq_t x     = S;
        seq_t dsum  = 0;
        while (x > 0) {
            dsum += x % 10;
            x   /= 10;
        }

        if (dsum == 0) {   /* should never happen for S>0 */
            k++;
            continue;
        }

        if (S % dsum == 0) {
            return k;
        }
        k++;
    }
}
