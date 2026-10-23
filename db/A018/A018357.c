/*
 * A018357  Divisors of 252.
 * https://oeis.org/A018357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 252 in ascending order (finite
 *            sequence)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static void init_divisors(seq_t *divs)
{
    /* Prime exponent counts: 2^2 · 3^2 · 7^1 */
    const seq_t pow2[3] = {(seq_t)1, (seq_t)2, (seq_t)4};
    const seq_t pow3[3] = {(seq_t)1, (seq_t)3, (seq_t)9};
    const seq_t pow7[2] = {(seq_t)1, (seq_t)7};

    size_t idx = 0;
    for (size_t a = 0; a < 3; ++a)
    {
        for (size_t b = 0; b < 3; ++b)
        {
            for (size_t c = 0; c < 2; ++c)
            {
                divs[idx++] = pow2[a] * pow3[b] * pow7[c];
            }
        }
    }

    /* Insertion sort – the array size is fixed (18) */
    for (size_t i = 1; i < 18; ++i)
    {
        seq_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j-1] > key)
        {
            divs[j] = divs[j-1];
            --j;
        }
        divs[j] = key;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static seq_t divs[18];
    static int initialized = 0;

    if (!initialized)
    {
        init_divisors(divs);
        initialized = 1;
    }

    size_t idx = (size_t)(n - 1);
    if (idx < 18) return divs[idx];
    return (seq_t)0;
}
