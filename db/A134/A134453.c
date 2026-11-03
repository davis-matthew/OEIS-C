/*
 * A134453  Numbers m such that the digital root in balanced ternary
 *          representation equals -1.
 * https://oeis.org/A134453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 971 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers m such that the digital root in balanced ternary representation equals -1. */

#include <stdbool.h>
#include <stddef.h>

static bool root_minus_one(seq_t n)
{
    seq_t t = n;
    long long sum = 0;
    while (t != 0)
    {
        seq_t r = t % 3;
        if (r == 2)
        {
            sum -= 1;
            t = t / 3 + 1;
        }
        else
        {
            sum += (long long)r;   /* r is 0 or 1 */
            t = t / 3;
        }
    }
    long long s = sum;
    while (s != -1 && s != 0 && s != 1)
    {
        long long m = s;
        long long new_sum = 0;
        while (m != 0)
        {
            long long r = m % 3;
            if (r == 2)
            {
                r = -1;
                m = m / 3 + 1;
            }
            else if (r == -2)
            {
                r = 1;
                m = m / 3 - 1;
            }
            else
            {
                m = m / 3;
            }
            new_sum += r;
        }
        s = new_sum;
    }
    return s == -1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    enum { MAX_CAP = 200000 };  /* sufficient for n up to about 10 000 */
    static seq_t cache[MAX_CAP];
    static size_t cache_len = 0;
    static seq_t next_candidate = 1;

    while (cache_len < (size_t)n)
    {
        if (root_minus_one(next_candidate))
        {
            if (cache_len < MAX_CAP)
                cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
