/*
 * A031485  Numbers whose base-7 representation has one fewer 0 than 6's.
 * https://oeis.org/A031485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A031485 (Numbers whose base‑7 representation
 * has one fewer 0 than 6's) by brute‑force enumeration. */
#include <stdlib.h>
#include <string.h>

/* static memoisation data */
static seq_t *cache = NULL;      /* array of found terms          */
static size_t cache_cap = 0;     /* allocated capacity             */
static size_t cache_size = 0;    /* number of terms stored         */
static seq_t next_value = 0;     /* next number to test (inclusive)*/

static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache && cache_size > 0)
    {
        memcpy(new_arr, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* iterate until we have at least n terms cached */
    while (cache_size < (size_t)n)
    {
        /* count zeros and sixes in base‑7 representation of next_value */
        size_t zero_cnt = 0, six_cnt = 0;
        seq_t cur = next_value;

        if (cur == (seq_t)0)
        {
            zero_cnt = 1;   /* representation "0" contains one zero */
        }
        else
        {
            seq_t tmp = cur;
            while (tmp != (seq_t)0)
            {
                seq_t digit = tmp % (seq_t)7;
                if (digit == (seq_t)0)
                    ++zero_cnt;
                else if (digit == (seq_t)6)
                    ++six_cnt;
                tmp /= (seq_t)7;
            }
        }

        if ((zero_cnt + 1) == six_cnt)
        {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = cur;
        }

        ++next_value;
    }

    return cache[(size_t)(n - 1)];
}
