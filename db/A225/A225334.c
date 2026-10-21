/*
 * A225334  Look-and-repeat: similar to look-and-say except frequency is
 *          repeated (seed 3).
 * https://oeis.org/A225334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Look‑and‑repeat sequence A225334
   Recurrence: a(1) = 3
   a(n+1) = lookandrepeat(a(n))
   For a run of length L of digit d, the run contributes
   the decimal representation of L repeated twice, followed by d.
   Example: 3 → 113 → 221113 → … */

#include <stdlib.h>

/* Convert a decimal string to seq_t.
   Called once for the final term. */
static seq_t str_to_seq(const char *s)
{
    seq_t val = 0;
    for (const char *p = s; *p; ++p)
    {
        val = val * ((seq_t)10) + (seq_t)(*p - '0');
    }
    return val;
}

seq_t a(idx_t n)
{
    /* Seed term */
    if (n == (idx_t)1)
        return (seq_t)3;

    /* Current term stored as a null‑terminated string */
    size_t cur_len = 1;           /* length without terminator */
    size_t cur_cap = 32;
    char *cur = (char *)malloc(cur_cap);
    if (!cur)
        return (seq_t)0;          /* allocation failed */
    cur[0] = '3';
    cur[1] = '\0';

    /* Compute terms up to n */
    for (idx_t step = (idx_t)2; step <= n; ++step)
    {
        /* Rough estimate of the capacity needed for the next term.
           Each digit may generate up to three digits (len twice + digit),
           so use a factor of 3 and some slack. */
        size_t next_cap = cur_len * 3 + 20;
        char *next = (char *)malloc(next_cap);
        if (!next)
        {
            free(cur);
            return (seq_t)0;
        }
        size_t pos = 0;

        /* Scan runs in the current term */
        for (size_t i = 0; i < cur_len; )
        {
            char digit = cur[i];
            size_t j = i + 1;
            while (j < cur_len && cur[j] == digit)
                ++j;
            size_t run_len = j - i;

            /* Convert run_len to decimal digits (most significant first) */
            char buf[32];
            int idx = 0;
            size_t tmp = run_len;
            do
            {
                buf[idx++] = (char)('0' + (tmp % 10));
                tmp /= 10;
            } while (tmp > 0);

            /* Append the length twice */
            for (int k = idx - 1; k >= 0; --k)
                next[pos++] = buf[k];
            for (int k = idx - 1; k >= 0; --k)
                next[pos++] = buf[k];

            /* Append the digit itself */
            next[pos++] = digit;

            i = j;
        }

        next[pos] = '\0';
        free(cur);
        cur = next;
        cur_len = pos;
    }

    /* Convert the final term string to seq_t */
    seq_t result = str_to_seq(cur);
    free(cur);
    return result;
}
