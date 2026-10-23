/*
 * A018630  Divisors of 759.
 * https://oeis.org/A018630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n‑th divisor of 759 (sorted ascending)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static cache for the eight divisors of 759 */
    static seq_t cache[16];
    static idx_t cache_size = 0;

    /* 759 is a constant */
    const idx_t N = 759;

    if (cache_size == 0) {
        seq_t temp[16];
        idx_t cnt = 0;

        /* compute ⌊√N⌋ */
        idx_t i;
        for (i = 1; i * i <= N; ++i);
        idx_t sqrtN = i - 1;

        for (i = 1; i <= sqrtN; ++i) {
            if (N % i == 0) {
                temp[cnt++] = (seq_t)i;
                if (i != N / i)
                    temp[cnt++] = (seq_t)(N / i);
            }
        }

        /* insertion sort on the small array of divisors */
        for (i = 1; i < cnt; ++i) {
            seq_t key = temp[i];
            idx_t j = i;
            while (j > 0 && temp[j - 1] > key) {
                temp[j] = temp[j - 1];
                --j;
            }
            temp[j] = key;
        }

        /* copy to the cache */
        for (i = 0; i < cnt; ++i)
            cache[i] = temp[i];
        cache_size = cnt;   /* should be 8 for 759 */
    }

    if (n < 1 || n > cache_size)
        return (seq_t)0;   /* undefined terms return 0 */

    return cache[n - 1];
}
