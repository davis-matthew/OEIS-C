/*
 * A100005  Bisection of A001414.
 * https://oeis.org/A100005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of prime factors of (2n‑1) counted with multiplicity
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = 2 * ((seq_t)n) - 1;   /* m = 2n‑1 */
    seq_t sum = 0;
    seq_t t = m;

    for (seq_t p = 2; p <= t / p; ++p) {
        while (t % p == 0) {
            sum += p;
            t /= p;
        }
    }

    if (t > 1) {
        sum += t;          /* remaining prime factor */
    }

    return sum;
}
