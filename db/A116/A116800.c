/*
 * A116800  Number of permutations of length n which avoid the patterns 1432,
 *          2314, 3241.
 * https://oeis.org/A116800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=72, a(6)=232,
 *            a(n)=7*a(n-1)-18*a(n-2)+22*a(n-3)-14*a(n-4)+3*a(n-5) for n>6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static const seq_t base[6] = {
        (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)21, (seq_t)72, (seq_t)232
    };

    if (n <= 6)
        return base[n - 1];

    seq_t arr[6];
    for (idx_t i = 0; i < 6; ++i)
        arr[i] = base[i];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 7 * arr[5]
                   - 18 * arr[4]
                   + 22 * arr[3]
                   - 14 * arr[2]
                   + 3  * arr[1];

        for (idx_t j = 0; j < 5; ++j)
            arr[j] = arr[j + 1];
        arr[5] = next;
    }

    return arr[5];
}
