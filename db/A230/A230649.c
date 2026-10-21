/*
 * A230649  Number of nX5 0..2 white square subarrays x(i,j) with each element
 *          diagonally or antidiagonally next to at least one element with
 *          value 2-x(i,j).
 * https://oeis.org/A230649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 59*a(n-2) - 230*a(n-4) - 2*a(n-6) + 32*a(n-8) Initial
 *            values: a(1) = 0, a(2) = 15, a(3) = 27, a(4) = 531, a(5) = 1635,
 *            a(6) = 28161, a(7) = 90393, a(8) = 1539207, a(9) = 4956927,
 *            a(10) = 84335553, a(11) = 271665969 (These are the only seed
 *            values needed to use the recurrence for n > 11.)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static cache for computed values up to a maximum reasonable n.
       The array is sufficiently large for n up to about 10,000 as
       requested.  Since seq_t may be a class type, we do not use
       malloc/realloc to avoid missing constructors. */
    static seq_t cache[10001];

    /* 0 is unused; store a(1) .. a(11) on first use. */
    static idx_t cache_len = 0;      /* cache_len == 1 + max index computed */
    if (cache_len == 0)
    {
        cache[0] = 0;
        cache[1] = 0;          /* a(1) */
        cache[2] = 15;         /* a(2) */
        cache[3] = 27;         /* a(3) */
        cache[4] = 531;        /* a(4) */
        cache[5] = 1635;       /* a(5) */
        cache[6] = 28161;      /* a(6) */
        cache[7] = 90393;      /* a(7) */
        cache[8] = 1539207;    /* a(8) */
        cache[9] = 4956927;    /* a(9) */
        cache[10] = 84335553;  /* a(10) */
        cache[11] = 271665969; /* a(11) */
        cache_len = 12;        /* next index to compute is 12 */
    }

    /* If requested value is already computed, return it. */
    if (n < cache_len)
        return cache[n];

    /* Compute values up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* a(i) = 59*a(i-2) - 230*a(i-4) - 2*a(i-6) + 32*a(i-8) */
        seq_t term1 = 59 * cache[i - 2];
        seq_t term2 = 230 * cache[i - 4];
        seq_t term3 = 2 * cache[i - 6];
        seq_t term4 = 32 * cache[i - 8];
        cache[i] = term1 + term4 - term2 - term3;
    }

    cache_len = n + 1;   /* update cache boundary */
    return cache[n];
}
