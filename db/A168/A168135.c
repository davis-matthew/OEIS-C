/*
 * A168135  Numbers expressible as 7*k+11*m (with nonnegative k, m) exactly in
 *          one way.
 * https://oeis.org/A168135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   numbers N such that N = 7*k + 11*m has exactly one nonnegative
 *            solution. For each N, let r = (8*N) mod 11 (since 7⁻¹ ≡ 8 (mod
 *            11)). There is precisely one solution iff N ≥ 7*r and N - 7*r <
 *            77
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t count = 0;
    /* All such integers satisfy N ≤ 136 (since 7*10 + 76 = 136). */
    for (idx_t N = 1; N <= 136; ++N) {
        seq_t value = (seq_t)N;
        seq_t r = ((seq_t)8 * value) % (seq_t)11;
        seq_t minN = 7 * r;
        if (value < minN)
            continue;
        if (value - minN < (seq_t)77) {          /* unique representation */
            ++count;
            if (count == n)
                return value;
        }
    }
    /* If n is larger than the number of unique values, return 0. */
    return 0;
}
