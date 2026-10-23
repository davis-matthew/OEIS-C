/*
 * A018368  Divisors of 275.
 * https://oeis.org/A018368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 275 in increasing order (finite sequence). */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Sorted array of all positive divisors of 275. */
static seq_t divisors[6];
static bool ready = false;

/* Initialize the divisor array once.  The algorithm works for any
   integer type seq_t that supports +, -, *, /, %, <, >, ==. */
static void init_divisors(void)
{
    if (ready)
        return;

    const seq_t N = 275;     /* 275 = 5^2 * 11^1 */
    seq_t temp[6];
    int cnt = 0;

    /* Find all divisors by testing i from 1 up to sqrt(N). */
    for (uint64_t i = 1; (seq_t)i * (seq_t)i <= N; ++i)
    {
        if (N % (seq_t)i == 0)
        {
            temp[cnt++] = (seq_t)i;
            seq_t other = N / (seq_t)i;
            if ((seq_t)i != other)
                temp[cnt++] = other;
        }
    }

    /* There should be exactly 6 divisors.  Sort them. */
    for (int i = 1; i < cnt; ++i)
    {
        seq_t key = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j] > key)
        {
            temp[j + 1] = temp[j];
            --j;
        }
        temp[j + 1] = key;
    }

    for (int i = 0; i < cnt; ++i)
        divisors[i] = temp[i];

    ready = true;
}

/* Return the n-th divisor of 275 (1‑based).  For n > 6 or n < 1 the
   function returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    init_divisors();

    if (n > 6)
        return 0;

    return divisors[(size_t)(n - 1)];
}
