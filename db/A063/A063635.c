/*
 * A063635  Smallest k such that 9^k has exactly n 9's in its decimal
 *          representation.
 * https://oeis.org/A063635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that 9^k has exactly n digits 9 in its
 *            decimal representation. (k starts from 1; k=0 is ignored for
 *            n=0, hence a(0)=2)
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation pre‑computes 9^k iteratively using base‑10000 digits.
   For each new power it counts the number of 9’s using a lookup table and
   records the first k producing a given count.  The cache is extended on demand
   so that the function can be called with any n in any order.  The algorithm
   runs in expected O(k²) digit operations; for n up to ~10 000 the required
   k is only about 10×n, so the run time stays well below the limits. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static int table9[10000];       /* number of 9’s in the 4‑digit decimal representation of i */

/* initialise table9 once */
static void init_table9(void)
{
    for (int i = 0; i < 10000; ++i)
    {
        int c = 0;
        int x = i;
        for (int d = 0; d < 4; ++d)
        {
            if (x % 10 == 9) ++c;
            x /= 10;
        }
        table9[i] = c;
    }
}

static void init_table9_once()
{
    static int initialized = 0;
    if (!initialized)
    {
        init_table9();
        initialized = 1;
    }
}

seq_t a(idx_t n)
{
    init_table9_once();

    /* static cache */
    static int *best = NULL;          /* best[count] = minimal k (>=1) */
    static size_t best_size = 0;
    static size_t max_computed_k = 0; /* last computed k (starting from 0) */
    static uint32_t *digits = NULL;   /* base 10000 digits of current 9^k */
    static size_t digits_len = 0;
    static int current_count = 0;     /* number of 9’s in current 9^k */

    if (best == NULL)
    {
        /* arbitrary initial size, will grow on demand */
        best_size = 10000;
        best = (int *)malloc(best_size * sizeof(int));
        for (size_t i = 0; i < best_size; ++i) best[i] = -1;
        digits = (uint32_t *)malloc(256 * sizeof(uint32_t));
        digits_len = 1;
        digits[0] = 1;          /* 9^0 = 1  (k=0) */
        current_count = 0;      /* has 0 nines */
        /* record for count 0 if needed (k will be set later when k>=2) */
        max_computed_k = 0;
    }

    /* handle n==0 specially (a(0)=2) */
    if (n == 0u) return (seq_t)2;

    /* extend best array if necessary */
    if ((size_t)n >= best_size)
    {
        size_t new_size = best_size * 2;
        while ((size_t)n >= new_size) new_size *= 2;
        best = (int *)realloc(best, new_size * sizeof(int));
        for (size_t i = best_size; i < new_size; ++i) best[i] = -1;
        best_size = new_size;
    }

    /* compute powers until we have found a(n) */
    while (best[(size_t)n] == -1)
    {
        /* advance to next k */
        ++max_computed_k;    /* now k = max_computed_k */

        /* multiply current 9^k-1 by 9 */
        uint64_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i)
        {
            uint64_t prod = (uint64_t)digits[i] * 9ULL + carry;
            digits[i] = (uint32_t)(prod % 10000ULL);
            carry = prod / 10000ULL;
        }
        while (carry > 0)
        {
            if (digits_len == 0)
            {
                digits = (uint32_t *)realloc(digits, 256 * sizeof(uint32_t));
                digits_len = 256;
            }
            digits[digits_len++] = (uint32_t)(carry % 10000ULL);
            carry /= 10000ULL;
        }

        /* count the number of 9’s in the new decimal representation */
        int cnt9 = 0;
        for (size_t i = 0; i < digits_len; ++i)
            cnt9 += table9[digits[i]];

        current_count = cnt9;

        /* record minimal k for this count (k>=1) */
        if (current_count < (int)best_size && best[(size_t)current_count] == -1)
            best[(size_t)current_count] = (int)max_computed_k;
    }

    return (seq_t)best[(size_t)n];
}
