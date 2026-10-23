/*
 * A047842  Describe n (count digits in order of increasing value, ignoring
 *          missing digits).
 * https://oeis.org/A047842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) is the digit‑count of n. For each digit d that occurs in n,
 *            output the decimal count of d followed by d, in increasing order
 *            of d. a(0)=10, a(1)=11, a(10)=1011, etc
 * Range:     u8 n <= 9, u16 n <= 101, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned int cnt[10] = {0};
    idx_t t = n;
    if (t == 0)
        cnt[0] = 1;
    else
        while (t > 0) {
            cnt[t % 10]++;   /* digit frequency */
            t /= 10;
        }

    seq_t result = 0;
    for (int d = 0; d < 10; ++d) {
        unsigned int c = cnt[d];
        if (c == 0)
            continue;

        /* number of decimal digits in the count c */
        int digits_c = (c < 10) ? 1 : (c < 100 ? 2 : 3);
        seq_t mult = 1;
        for (int i = 0; i < digits_c + 1; ++i)
            mult *= (seq_t)10;          /* 10^(digits_c+1) */

        seq_t part = ((seq_t)c) * 10 + (seq_t)d;
        result = result * mult + part;
    }
    return result;
}
