/*
 * A018564  Divisors of 639.
 * https://oeis.org/A018564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 639 in increasing order (a(1)=1,
 *            a(2)=3, …, a(6)=639)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 639 = 3^2 * 71^1, so there are 6 divisors */
    if (n == 0 || n > 6)
        return 0;

    const seq_t primes[2] = {3, 71};
    const int exps[2]    = {2, 1};
    const int num_factors = 2;
    const int total_divs = 6;          /* (2+1)*(1+1) */

    seq_t divs[total_divs];
    int count = 0;

    /* iterate over exponents of each prime factor to generate all divisors */
    for (int e0 = 0; e0 <= exps[0]; ++e0)
    {
        seq_t pow0 = 1;
        for (int i = 0; i < e0; ++i)
            pow0 *= (seq_t)primes[0];

        for (int e1 = 0; e1 <= exps[1]; ++e1)
        {
            seq_t pow1 = 1;
            for (int i = 0; i < e1; ++i)
                pow1 *= (seq_t)primes[1];

            divs[count++] = pow0 * pow1;
        }
    }

    /* insertion sort – only 6 elements, trivial */
    for (int i = 1; i < total_divs; ++i)
    {
        seq_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key)
        {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    return divs[(int)(n - 1)];
}
