/*
 * A130202  Odious triangular numbers.
 * https://oeis.org/A130202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th triangular number that is odious (a triangular
 *            number T_k = k*(k+1)/2 with an odd number of 1's in its binary
 *            representation)
 * Range:     u8 n <= 7, u16 n <= 161, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int popcount_seq(seq_t x)
{
    int cnt = 0;
    while (x) {
        x &= x - 1;
        cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t k = 1;
    while (1) {
        seq_t t = ((seq_t)k * (k + 1)) / 2;
        if ((popcount_seq(t) & 1) == 1) {
            found++;
            if (found == n)
                return t;
        }
        k++;
    }
}
