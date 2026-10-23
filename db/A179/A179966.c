/*
 * A179966  Number of permutations p of [2*n+8] such that |p(i)-p(i-1)| > n
 *          for i>1.
 * https://oeis.org/A179966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (978034131/32768)*16^n * - (1954858304/253125)*15^n * -
 *            (174619/2592)*12^n * - (576/16807)*7^n for n > 4 * with a(0) …
 *            a(4) given explicitly
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 12, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_small(int base, idx_t exp)
{
    seq_t res = 1;
    seq_t b = (seq_t)base;
    for (idx_t i = 0; i < exp; ++i)
        res *= b;
    return res;
}

seq_t a(idx_t n)
{
    /* initial values for n = 0 … 4 */
    static const seq_t base[5] = {
        (seq_t)40320, (seq_t)479306, (seq_t)6564318,
        (seq_t)99133496, (seq_t)1572313392
    };
    if (n < 5)
        return base[n];

    seq_t part1 = (seq_t)978034131 *
                  pow_small(2, (int)(4*n - 15));

    seq_t part2 = (seq_t)1954858304;
    part2 *= pow_small(3, (int)(n - 4));
    part2 *= pow_small(5, (int)(n - 5));

    seq_t part3 = (seq_t)174619;
    part3 *= pow_small(2, (int)(2*n - 5));
    part3 *= pow_small(3, (int)(n - 4));

    seq_t part4 = (seq_t)576 * pow_small(7, (int)(n - 5));

    return part1 - part2 - part3 - part4;
}
