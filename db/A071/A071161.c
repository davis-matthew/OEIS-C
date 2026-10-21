/*
 * A071161  Integers whose decimal expansion satisfies the condition that if
 *          we read each term from the left to right (the most significant to
 *          the least significant digit) then each nonzero digit gives a
 *          distance to the next nonzero digit to right (with a cyclic
 *          wrap-over from the least-significant to the mo...
 * https://oeis.org/A071161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071161: Numbers whose non‑zero digits give the cyclic
   distances to the next non‑zero digit.
   The implementation enumerates all restricted compositions of a
   length L (parts 1–9), unranking the (n‑1)‑th composition in
   lexicographic order, and then builds the decimal number from the
   composition.  It works for any seq_t implementation that supports
   the ordinary integer operators. */

#include <stdlib.h>
#include <stdint.h>

/* temporary storage for the number of compositions of each sum */
static idx_t *ways_arr = NULL;   /* ways_arr[s] = number of compositions of s */
static idx_t ways_len = 0;       /* current allocated size (>= needed + 1) */

/* ensure that ways_arr has entries up to 'needed' (inclusive) */
static void ensure_ways(idx_t needed)
{
    if (ways_len > needed) return;          /* already present */

    /* grow allocation */
    idx_t new_len = ways_len ? ways_len : 1;
    while (new_len <= needed) new_len <<= 1;

    ways_arr = (idx_t *)realloc(ways_arr, new_len * sizeof(idx_t));

    /* zero the newly allocated space */
    for (idx_t i = ways_len; i < new_len; ++i) ways_arr[i] = 0;

    if (ways_len == 0)
    {
        ways_arr[0] = 1;   /* empty composition of sum 0 */
        ways_len = 1;
    }

    /* compute counts for the new entries */
    for (idx_t s = ways_len; s < new_len; ++s)
    {
        idx_t sum = 0;
        for (idx_t part = 1; part <= 9 && part <= s; ++part)
            sum += ways_arr[s - part];
        ways_arr[s] = sum;
    }
    ways_len = new_len;
}

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* offset zero */
    if (n == 0) return (seq_t)0;

    /* target is 0‑based among all non‑zero terms */
    idx_t target = n - 1;

    /* find the digit length L that contains this target */
    idx_t cum   = 0;   /* cumulative count up to previous length */
    idx_t L     = 0;   /* current length being considered */

    while (1)
    {
        ++L;
        ensure_ways(L);
        idx_t cnt = ways_arr[L];
        if (cum + cnt > target)
        {
            target -= cum;   /* adjust to index within this length */
            break;
        }
        cum += cnt;
    }

    /* unrank the target‑th composition of sum L (parts 1..9) */
    idx_t remaining = L;
    idx_t offset    = target;
    int   parts[100];          /* enough for L ≤ 100 (our limits are far below) */
    idx_t pidx = 0;

    while (remaining > 0)
    {
        for (idx_t val = 1; val <= 9 && val <= remaining; ++val)
        {
            idx_t cnt = ways_arr[remaining - val];
            if (offset < cnt)
            {
                parts[pidx++] = (int)val;
                remaining -= val;
                break;
            }
            offset -= cnt;
        }
    }

    /* construct decimal number from the composition */
    seq_t result = 0;
    for (idx_t i = 0; i < pidx; ++i)
    {
        idx_t d = (idx_t)parts[i];
        result = result * (seq_t)10 + (seq_t)d;
        for (idx_t z = 0; z < d - 1; ++z)
            result *= (seq_t)10;            /* append zeros */
    }

    return result;
}
