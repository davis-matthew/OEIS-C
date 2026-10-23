/*
 * A018759  Divisors of 986.
 * https://oeis.org/A018759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 986
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          986 = 2 × 17 × 29,  so the divisors are 1, 2, 17, 29, 34, 58, 493, 986. */
seq_t a(idx_t n)
{
    /* n is 1‑based.  For n > 8 we return 0 (undefined). */
    if (n == 0) return 0;

    /* Prime factors of 986 */
    static const seq_t primes[3] = { (seq_t)2, (seq_t)17, (seq_t)29 };

    /* There are only 8 divisors.  Compute them by exhausting the bit masks 0..7. */
    seq_t divs[8];
    int cnt = 0;
    for (int mask = 0; mask < 8; ++mask)
    {
        seq_t d = (seq_t)1;
        if (mask & 1) d = d * primes[0];
        if (mask & 2) d = d * primes[1];
        if (mask & 4) d = d * primes[2];
        divs[cnt++] = d;
    }

    /* Simple insertion sort for such a tiny array. */
    for (int i = 1; i < cnt; ++i)
    {
        seq_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key)
        {
            divs[j + 1] = divs[j];
            j--;
        }
        divs[j + 1] = key;
    }

    if (n <= (idx_t)cnt)
        return divs[n - 1];
    else
        return (seq_t)0;
}
