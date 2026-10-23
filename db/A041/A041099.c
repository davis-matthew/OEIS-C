/*
 * A041099  Denominators of continued fraction convergents to sqrt(57).
 * https://oeis.org/A041099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 302*a(n-6) - a(n-12) (n≥12) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=9, a(4)=11, a(5)=20, a(6)=291, a(7)=311,
 *            a(8)=602, a(9)=2719, a(10)=3321, a(11)=6040
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sufficient capacity for n up to ~10000 */
    static seq_t cache[20000];
    static idx_t cache_len = 0;

    /* lazy initialization of the first 12 terms */
    if (cache_len == 0) {
        static const seq_t init[12] = {
            (seq_t)1ULL, (seq_t)1ULL, (seq_t)2ULL, (seq_t)9ULL,
            (seq_t)11ULL, (seq_t)20ULL, (seq_t)291ULL, (seq_t)311ULL,
            (seq_t)602ULL, (seq_t)2719ULL, (seq_t)3321ULL, (seq_t)6040ULL
        };
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_len = 12;
    }

    if (n < cache_len)
        return cache[n];

    /* compute missing terms up to n (recurrence valid for n>=12) */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 6] * (seq_t)302 - cache[i - 12];
    }
    cache_len = n + 1;
    return cache[n];
}
