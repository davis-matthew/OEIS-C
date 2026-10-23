/*
 * A021932  Decimal expansion of 1/928.
 * https://oeis.org/A021932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor((10^(n+1)) / 928) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (nth decimal digit of 1/928 after the decimal point) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute the decimal digits of 1/928 and detect the period. */
    static int initialized = 0;
    static idx_t preperiod = 0;
    static idx_t period   = 0;
    static seq_t digits[928];   /* maximum possible number of digits before repeat */
    static int  seen[928];      /* index of first occurrence of each remainder */

    if (!initialized)
    {
        /* initialise the seen table with -1 */
        for (int i = 0; i < 928; ++i)
            seen[i] = -1;

        uint64_t rem = 1;    /* remainder initially 1 */
        idx_t pos = 0;       /* digit position */

        while (seen[rem] == -1)        /* until a remainder repeats */
        {
            seen[rem] = pos;
            uint64_t q = (rem * 10) / 928;      /* the next digit */
            digits[pos] = (seq_t)q;
            rem = (rem * 10) % 928;             /* new remainder */
            pos++;
        }

        preperiod = seen[rem];      /* start of the repeating part */
        period   = pos - preperiod; /* length of the repeating cycle */
        initialized = 1;
    }

    /* return the requested digit */
    if (n < (idx_t)(preperiod + period))
        return digits[n];
    else
    {
        idx_t idx = preperiod + ((n - preperiod) % period);
        return digits[idx];
    }
}
