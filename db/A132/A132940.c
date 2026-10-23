/*
 * A132940  Concatenation of first n Fermat primes.
 * https://oeis.org/A132940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   concatenation of the first n Fermat primes (3,5,17,257,65537)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static int digit_count(seq_t x)
{
    int cnt = 0;
    do
    {
        cnt++;
        x /= 10;
    } while (x);
    return cnt;
}

seq_t a(idx_t n)
{
    /* Fermat primes (integers large enough to hold all known values) */
    static const uint32_t fermat_primes_int[] = {3U, 5U, 17U, 257U, 65537U};
    const size_t prime_count = sizeof fermat_primes_int / sizeof fermat_primes_int[0];

    if (n == 0 || n > prime_count)
        return 0;

    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t p   = (seq_t)fermat_primes_int[i];
        int     plen = digit_count(p);

        /* compute 10^plen in seq_t */
        seq_t power = 1;
        for (int j = 0; j < plen; ++j)
            power *= 10;

        result = result * power + p;
    }
    return result;
}
