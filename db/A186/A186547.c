/*
 * A186547  1/8192 the number of (n+2)X3 0..3 arrays with each 3X3 subblock
 *          trace equal to some horizontal or vertical neighbor 3X3 subblock
 *          trace.
 * https://oeis.org/A186547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A186547:
   a(n) = 32*a(n-1) - 77*a(n-2) - 4496*a(n-3) + 117783*a(n-4)
          - 1083636*a(n-5) + 7006716*a(n-6) - 23429520*a(n-7)
          + 57318192*a(n-8) - 54976320*a(n-9) + 41990400*a(n-10)
   Initial values (n = 1..10):
   a(1)=0  a(2)=290  a(3)=2972  a(4)=168200
   a(5)=3090668  a(6)=111188198  a(7)=2585394080
   a(8)=78931270418  a(9)=2022581728964  a(10)=57893438838608
*/
#include <stdlib.h>

static seq_t *seq_cache = NULL;     /* cache 1‑based, index 0 unused */
static idx_t  seq_cache_max = 0;     /* last computed index */

/* Ensure the cache contains values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= seq_cache_max)
        return;

    size_t new_size = (size_t)(n + 1);          /* indices 0..n */
    seq_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!seq_cache)
        exit(1); /* out of memory */

    for (idx_t i = seq_cache_max + 1; i <= n; ++i)
    {
        /* initial values for i = 1..10 */
        switch ((int)i)
        {
            case 1:  seq_cache[i] = (seq_t)0;                    break;
            case 2:  seq_cache[i] = (seq_t)290;                  break;
            case 3:  seq_cache[i] = (seq_t)2972;                 break;
            case 4:  seq_cache[i] = (seq_t)168200;               break;
            case 5:  seq_cache[i] = (seq_t)3090668;              break;
            case 6:  seq_cache[i] = (seq_t)111188198;            break;
            case 7:  seq_cache[i] = (seq_t)2585394080;           break;
            case 8:  seq_cache[i] = (seq_t)78931270418ULL;       break;
            case 9:  seq_cache[i] = (seq_t)2022581728964ULL;     break;
            case 10: seq_cache[i] = (seq_t)57893438838608ULL;    break;
            default:
            {
                /* recurrence for i > 10 */
                seq_t a1  = seq_cache[i - 1];
                seq_t a2  = seq_cache[i - 2];
                seq_t a3  = seq_cache[i - 3];
                seq_t a4  = seq_cache[i - 4];
                seq_t a5  = seq_cache[i - 5];
                seq_t a6  = seq_cache[i - 6];
                seq_t a7  = seq_cache[i - 7];
                seq_t a8  = seq_cache[i - 8];
                seq_t a9  = seq_cache[i - 9];
                seq_t a10 = seq_cache[i -10];

                seq_t val = (seq_t)32 * a1
                          - (seq_t)77 * a2
                          - (seq_t)4496 * a3
                          + (seq_t)117783 * a4
                          - (seq_t)1083636 * a5
                          + (seq_t)7006716 * a6
                          - (seq_t)23429520 * a7
                          + (seq_t)57318192 * a8
                          - (seq_t)54976320 * a9
                          + (seq_t)41990400 * a10;

                seq_cache[i] = val;
                break;
            }
        }
    }
    seq_cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return seq_cache[(size_t)n];
}
