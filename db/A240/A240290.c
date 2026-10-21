/*
 * A240290  Number of n X 2 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it or two plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4, a(2) = 6, a(3) = 24, a(4) = 56, a(5) = 230, a(6) =
 *            552, a(7) = 2270, a(8) = 5456, a(9) = 22416. For n > 9: a(n) =
 *            12*a(n-2) - 24*a(n-4) + 31*a(n-6) - 16*a(n-8)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cache[20001];          /* storage for up to n = 20000 */
static idx_t cache_len = 0;         /* number of terms already computed */

static void init_cache(void)
{
    if (cache_len != 0)
        return;                     /* already initialized */

    cache[0] = 4;
    cache[1] = 6;
    cache[2] = 24;
    cache[3] = 56;
    cache[4] = 230;
    cache[5] = 552;
    cache[6] = 2270;
    cache[7] = 5456;
    cache[8] = 22416;
    cache_len = 9;
}

static void compute_up_to(idx_t n)
{
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* recurrence indices are 1‑based; cache is 0‑based */
        seq_t a_i_2 = cache[i - 2 - 1];
        seq_t a_i_4 = cache[i - 4 - 1];
        seq_t a_i_6 = cache[i - 6 - 1];
        seq_t a_i_8 = cache[i - 8 - 1];

        cache[i - 1] =
            (12ULL * a_i_2) - (24ULL * a_i_4) + (31ULL * a_i_6) - (16ULL * a_i_8);
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n < 1, return 0 */

    init_cache();

    if (n <= cache_len)
        return cache[n - 1];

    compute_up_to(n);
    return cache[n - 1];
}
