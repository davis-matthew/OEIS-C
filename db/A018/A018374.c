/*
 * A018374  Divisors of 285.
 * https://oeis.org/A018374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 285 in ascending order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int divisors_initialized = 0;
static seq_t divisors[8];

static void generate_divisors(void)
{
    /* generate all 8 divisors from 285 = 3*5*19 */
    int i, j, k, pos = 0;
    for(i = 0; i <= 1; ++i)
        for(j = 0; j <= 1; ++j)
            for(k = 0; k <= 1; ++k)
            {
                seq_t val = (seq_t)1;
                if(i) val *= (seq_t)3;
                if(j) val *= (seq_t)5;
                if(k) val *= (seq_t)19;
                divisors[pos++] = val;
            }
    /* sort the 8 values (insertion sort) */
    for(i = 1; i < 8; ++i)
    {
        seq_t key = divisors[i];
        j = i - 1;
        while(j >= 0 && divisors[j] > key)
        {
            divisors[j + 1] = divisors[j];
            --j;
        }
        divisors[j + 1] = key;
    }
    divisors_initialized = 1;
}

seq_t a(idx_t n)
{
    if(n < 1) return (seq_t)0;
    if(!divisors_initialized) generate_divisors();
    if(n <= 8u) return divisors[n - 1];
    return (seq_t)0;
}
