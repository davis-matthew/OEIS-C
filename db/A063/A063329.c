/*
 * A063329  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          56 ).
 * https://oeis.org/A063329
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-2) + a(n-4) - a(n-6) for n > 8 * Initial values
 *            (offset 2): a(2) = -1, a(3) = 42, a(4) = 94, a(5) = 144, * a(6)
 *            = 198, a(7) = 248, a(8) = 302 * * This implementation uses a
 *            static array of size 10001, * sufficient for all n up to about
 *            10 000 as required. * It works in both C11 and C++...
 * Range:     i8 n <= 4, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 2, return 0 for safety */
    if (n < 2)
        return (seq_t)0;

    /* Reserve space for n up to 10000 */
    /* NOLINTNEXTLINE: static array size fixed by requirement */
    static seq_t cache[10001];
    static idx_t current_max = 1;   /* up to which index we have computed */

    /* Set up base values once */
    if (current_max == 1) {
        /* Zero out unused entries */
        for (idx_t i = 0; i <= 1; ++i)
            cache[i] = (seq_t)0;

        cache[2] = (seq_t)(-1);
        cache[3] = (seq_t)(42);
        cache[4] = (seq_t)(94);
        cache[5] = (seq_t)(144);
        cache[6] = (seq_t)(198);
        cache[7] = (seq_t)(248);
        cache[8] = (seq_t)(302);
        current_max = 8;
    }

    /* Extend computation up to n */
    if (n > current_max) {
        for (idx_t i = current_max + 1; i <= n; ++i) {
            /* recurrence applies for i > 8, but still works for 9.. */
            cache[i] = cache[i - 2] + cache[i - 4] - cache[i - 6];
        }
        current_max = n;
    }

    return cache[n];
}
