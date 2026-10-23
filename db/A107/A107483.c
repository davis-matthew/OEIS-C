/*
 * A107483  Base 3 representation of the positive integers n such that
 *          Sum[d[k],k=1..n] is an integer, where d(k) is the base 3 fraction
 *          0.k (e.g., d(22 base 10)=d(211 base 3)=0.221 base 3).
 * https://oeis.org/A107483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Let k = ⌊n/4⌋. • If n ≡ 1 (mod 4), output a decimal number with
 *            3k + 1 digits all equal to 2. • If n ≡ 2 (mod 4), output 2
 *            followed by 3k + 1 zero digits. • If n ≡ 3 (mod 4), output 1
 *            followed by 3k + 2 zero digits. • If n ≡ 0 (mod 4) (n ≥ 4),
 *            output 1 followed by 3k − 1 two digits
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 4;
    int r = n % 4;
    seq_t val = 0;

    switch (r) {
    case 1: {
        idx_t L = 3 * k + 1;
        for (idx_t i = 0; i < L; ++i)
            val = val * 10 + 2;
        break;
    }
    case 2: {
        idx_t L = 3 * k + 2;
        val = 2;
        for (idx_t i = 1; i < L; ++i)
            val *= 10;
        break;
    }
    case 3: {
        idx_t L = 3 * k + 3;
        val = 1;
        for (idx_t i = 1; i < L; ++i)
            val *= 10;
        break;
    }
    case 0: /* n ≥ 4 */
    {
        idx_t L = 3 * k;   /* k ≥ 1 */
        val = 1;
        for (idx_t i = 1; i < L; ++i)
            val = val * 10 + 2;
        break;
    }
    }
    return val;
}
