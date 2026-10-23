/*
 * A008467  a(n) = n OR n^2 (applied to ternary expansions).
 * https://oeis.org/A008467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = digitwise OR (max) of n and n² in base‑3
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t sq = n_seq * n_seq;          /* n² */
    seq_t res = 0;
    seq_t pow = 1;
    const seq_t THREE = 3;

    while (n_seq > 0 || sq > 0) {
        seq_t dn  = n_seq % THREE;
        seq_t dsq = sq   % THREE;
        seq_t d_or = dn > dsq ? dn : dsq;

        res += d_or * pow;
        pow *= THREE;

        n_seq /= THREE;
        sq    /= THREE;
    }

    return res;
}
