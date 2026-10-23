/*
 * A018565  Divisors of 640.
 * https://oeis.org/A018565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest positive divisor of 640 (sorted ascending)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is finite: divisors of 640 = 2⁷·5, there are 16 of them. */
    if (n == 0) return 0;  /* offset starts at 1 */

    const unsigned int total = 16;      /* (7+1)*(1+1) divisors */

    /* Generate all divisors and store them in a small array. */
    seq_t vals[total];
    int pos = 0;

    seq_t pow2[8];
    seq_t pow5[2];

    pow2[0] = (seq_t)1;
    for (int i = 1; i <= 7; ++i)
        pow2[i] = pow2[i-1] * (seq_t)2;   /* 2^i */

    pow5[0] = (seq_t)1;
    pow5[1] = pow5[0] * (seq_t)5;         /* 5^j */

    for (int i = 0; i <= 7; ++i)
        for (int j = 0; j <= 1; ++j)
            vals[pos++] = pow2[i] * pow5[j];

    /* Sort the 16 values in ascending order. */
    for (int i = 0; i < (int)total - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < (int)total; ++j)
            if (vals[j] < vals[minIdx])
                minIdx = j;
        if (minIdx != i) {
            seq_t tmp = vals[i];
            vals[i] = vals[minIdx];
            vals[minIdx] = tmp;
        }
    }

    if (n > (idx_t)total)
        return 0;   /* undefined for indices beyond the finite sequence */

    return vals[(int)n - 1];
}
