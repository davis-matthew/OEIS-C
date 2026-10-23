/*
 * A018603  Divisors of 710.
 * https://oeis.org/A018603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth smallest divisor of 710 (sorted ascending)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t divisors[8];
static size_t divisor_count;
static int initialized = 0;

/* Compute the divisors of 710 and store them sorted in ascending order. */
static void init_divisors(void)
{
    seq_t temp[8];
    size_t cnt = 0;
    seq_t N = (seq_t)710;

    /* Find divisors by trial division up to sqrt(N). */
    for (seq_t d = (seq_t)1; d * d <= N; ++d)
    {
        if (N % d == 0)
        {
            temp[cnt++] = d;
            if (d != N / d)
                temp[cnt++] = N / d;
        }
    }

    /* Simple insertion sort – works with any seq_t that supports operator< >. */
    for (size_t i = 1; i < cnt; ++i)
    {
        seq_t key = temp[i];
        size_t j = i;
        while (j > 0 && temp[j - 1] > key)
        {
            temp[j] = temp[j - 1];
            --j;
        }
        temp[j] = key;
    }

    /* Copy sorted divisors to the static array. */
    for (size_t i = 0; i < cnt; ++i)
        divisors[i] = temp[i];

    divisor_count = cnt;
}

/* Return the nth term of the sequence (a(1) = 1). */
seq_t a(idx_t n)
{
    if (!initialized)
    {
        init_divisors();
        initialized = 1;
    }

    if (n == 0)
        return (seq_t)0;                     /* undefined for n<1, return 0 */

    /* n is unsigned, so n-1 is safe as long as n>0 */
    if ((idx_t)(n - 1) < divisor_count)
        return divisors[n - 1];

    return (seq_t)0;                         /* beyond the list of divisors */
}
