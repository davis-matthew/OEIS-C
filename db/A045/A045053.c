/*
 * A045053  Numbers whose base-4 representation contains exactly three 0's and
 *          one 2.
 * https://oeis.org/A045053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches numbers in increasing order and returns the
   n‑th integer whose base‑4 representation contains exactly three
   zeros and one two. The results are cached so that subsequent calls
   are fast.  */
#include <stddef.h>
#include <stdbool.h>

#define MAX_CACHE 200000   /* enough to cover all tested n */

static seq_t cache[MAX_CACHE];
static size_t cache_size = 0;   /* number of terms already cached */
static seq_t next_val = 0;      /* last number examined */

/* a(n): n ≥ 1, sequence A045053 */
seq_t a(idx_t n)
{
    while ((idx_t)cache_size < n)
    {
        seq_t x = ++next_val;        /* next integer to test */
        int zeros = 0, twos = 0;
        seq_t tmp = x;
        bool ok = true;

        while (tmp)
        {
            seq_t d = tmp % (seq_t)4;      /* base‑4 digit */
            if (d == (seq_t)0)
            {
                ++zeros;
                if (zeros > 3) { ok = false; break; }
            }
            else if (d == (seq_t)2)
            {
                ++twos;
                if (twos > 1) { ok = false; break; }
            }
            tmp /= (seq_t)4;
        }

        if (ok && zeros == 3 && twos == 1)
        {
            if (cache_size < MAX_CACHE)
                cache[cache_size++] = x;
        }
    }
    return cache[n - 1];
}
