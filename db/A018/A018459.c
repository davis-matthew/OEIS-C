/*
 * A018459  Divisors of 442.
 * https://oeis.org/A018459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 442 in increasing order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


static seq_t s_divisors[8];
static size_t s_div_cnt = 0;
static int s_div_inited = 0;

static void init_divisors(void)
{
    if (s_div_inited)
        return;
    s_div_inited = 1;                    /* avoid recursion issues */

    /* Generate all 2^3 = 8 divisors of 442 = 2 * 13 * 17 */
    size_t cnt = 0;
    for (int e2 = 0; e2 <= 1; ++e2)
    {
        for (int e13 = 0; e13 <= 1; ++e13)
        {
            for (int e17 = 0; e17 <= 1; ++e17)
            {
                unsigned int val = 1U;
                if (e2)
                    val *= 2U;
                if (e13)
                    val *= 13U;
                if (e17)
                    val *= 17U;
                s_divisors[cnt++] = (seq_t)val;   /* cast to seq_t */
            }
        }
    }
    s_div_cnt = cnt;

    /* Insertion sort the 8 divisors (simple, O(k^2) where k=8) */
    for (size_t i = 1; i < s_div_cnt; ++i)
    {
        seq_t key = s_divisors[i];
        size_t j = i;
        while (j > 0 && s_divisors[j - 1] > key)
        {
            s_divisors[j] = s_divisors[j - 1];
            --j;
        }
        s_divisors[j] = key;
    }
}

seq_t a(idx_t n)
{
    init_divisors();                  /* ensure divisors are ready */

    if (n == 0)          /* offset is 1, guard for undefined */
        return 0;

    if (n <= s_div_cnt)
        return s_divisors[(size_t)(n - 1)];
    return 0;                     /* no further divisors */
}
