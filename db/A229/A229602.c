/*
 * A229602  Number of defective 3-colorings of an n X 4 0..2 array connected
 *          horizontally and vertically with exactly two mistakes, and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for n > 11:
   a(n) = 27*a(n-1) - 288*a(n-2) + 1557*a(n-3) - 4644*a(n-4) +
          8073*a(n-5) - 8343*a(n-6) + 5022*a(n-7) - 1620*a(n-8) + 216*a(n-9);
   Initial terms:
   a(1) =  3
   a(2) = 202
   a(3) = 3040
   a(4) = 35182
   a(5) = 362100
   a(6) = 3476928
   a(7) = 31848813
   a(8) = 281845934
   a(9) = 2429033768
   a(10)= 20498682291
   a(11)= 170050970491
*/
seq_t a(idx_t n)
{
    /* Static cache up to n = 10000, enough for the required range. */
    static seq_t cache[10001] = {0};
    static idx_t cache_len = 0;
    static int initialized = 0;

    if (!initialized)
    {
        /* Pre‑populate the first 11 terms. */
        static const seq_t init_vals[12] = {
            0,
            3, 202, 3040, 35182,
            362100, 3476928, 31848813,
            281845934, 2429033768,
            20498682291, 170050970491
        };
        for (idx_t i = 1; i <= 11; i++)
            cache[i] = init_vals[i];
        cache_len = 11;
        initialized = 1;
    }

    if (n <= cache_len)
        return cache[n];

    /* Compute terms iteratively up to n using the recurrence. */
    for (idx_t k = cache_len + 1; k <= n; ++k)
    {
        seq_t pos = 0, neg = 0;
        pos += (seq_t)27  * cache[k - 1];
        neg += (seq_t)288 * cache[k - 2];
        pos += (seq_t)1557 * cache[k - 3];
        neg += (seq_t)4644 * cache[k - 4];
        pos += (seq_t)8073 * cache[k - 5];
        neg += (seq_t)8343 * cache[k - 6];
        pos += (seq_t)5022 * cache[k - 7];
        neg += (seq_t)1620 * cache[k - 8];
        pos += (seq_t)216  * cache[k - 9];
        cache[k] = pos - neg;
    }
    cache_len = n;
    return cache[n];
}
