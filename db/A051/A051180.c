/*
 * A051180  Number of 3-element intersecting families of an n-element set.
 * https://oeis.org/A051180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 343*a(n-2) + 2135*a(n-3) - 7504*a(n-4) +
 *            14756*a(n-5) - 14832*a(n-6) + 5760*a(n-7); a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=13, a(4)=222, a(5)=2585, a(6)=25830;
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mul_seq(seq_t a, unsigned int m)
{
#if defined(__cplusplus)
    return a * m;                      /* works for builtin types and for arbitrary‑precision C++ types */
#else
    return (seq_t)((unsigned long long)a * m);   /* safe for all builtin unsigned types */
#endif
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2) return 0;
    if (n == 3) return 13;
    if (n == 4) return 222;
    if (n == 5) return 2585;
    if (n == 6) return 25830;

    seq_t v[7] = {0, 0, 0, 13, 222, 2585, 25830};

    for (idx_t i = 7; i <= n; ++i) {
        seq_t val = 0;
        val += mul_seq(v[6], 29);
        val -= mul_seq(v[5], 343);
        val += mul_seq(v[4], 2135);
        val -= mul_seq(v[3], 7504);
        val += mul_seq(v[2], 14756);
        val -= mul_seq(v[1], 14832);
        val += mul_seq(v[0], 5760);

        /* shift the sliding window */
        for (int j = 0; j < 6; ++j) v[j] = v[j + 1];
        v[6] = val;
    }

    return v[6];
}
