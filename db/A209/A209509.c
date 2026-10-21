/*
 * A209509  Half the number of (n+1)X7 0..2 arrays with every 2X2 subblock
 *          having two or four distinct clockwise edge differences.
 * https://oeis.org/A209509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence for A209509
 *
 * a(1) = 2187,
 * a(2) = 53247,
 * a(3) = 1323270,
 * a(4) = 33137271,
 * a(5) = 832374135,
 * a(6) = 20934997854,
 * a(7) = 526815563193,
 * a(8) = 13259938040553,
 * a(9) = 333783842574072,
 * a(10) = 8402458085977977,
 * a(11) = 211521567465170343,
 * a(12) = 5324833273885068690,
 * a(13) = 134047481752728606261
 *
 * for n > 13:
 *   a(n) = 55*a(n-1) -1109*a(n-2) +11330*a(n-3) -67206*a(n-4)
 *        +247404*a(n-5) -582440*a(n-6) +881876*a(n-7)
 *        -846764*a(n-8) +499200*a(n-9) -172400*a(n-10)
 *        +33152*a(n-11) -3264*a(n-12) +128*a(n-13)
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* value for n=0 is outside of the sequence offset */
        return (seq_t)0;
    }

    /* static cache for memoised values */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* static array holding the first 13 terms */
    static seq_t init[13];
    static bool init_done = false;

    if (!init_done) {
        init_done = true;
        const char *init_strs[13] = {
            "2187",
            "53247",
            "1323270",
            "33137271",
            "832374135",
            "20934997854",
            "526815563193",
            "13259938040553",
            "333783842574072",
            "8402458085977977",
            "211521567465170343",
            "5324833273885068690",
            "134047481752728606261"
        };
        for (int i = 0; i < 13; ++i) {
            init[i] = seq_from_str(init_strs[i]);
        }
    }

    size_t target = (size_t)n;          /* n is at least 1 */

    /* grow the cache if necessary */
    if (target > cache_cap) {
        size_t newcap = cache_cap ? (cache_cap << 1) : 16;
        while (newcap < target) newcap <<= 1;
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newptr) abort(); /* memory allocation failure */
        cache = newptr;
        cache_cap = newcap;
    }

    /* compute missing values */
    if (target > cache_len) {
        for (size_t i = cache_len; i < target; ++i) {
            if (i < 13) {
                cache[i] = init[i]; /* copy the known initial terms */
            } else {
                /* linear recurrence for n > 13 */
                seq_t val =   55 * cache[i - 1];
                val        -= 1109 * cache[i - 2];
                val        += 11330 * cache[i - 3];
                val        -= 67206 * cache[i - 4];
                val        += 247404 * cache[i - 5];
                val        -= 582440 * cache[i - 6];
                val        += 881876 * cache[i - 7];
                val        -= 846764 * cache[i - 8];
                val        += 499200 * cache[i - 9];
                val        -= 172400 * cache[i -10];
                val        +=  33152 * cache[i -11];
                val        -=   3264 * cache[i -12];
                val        +=    128 * cache[i -13];
                cache[i] = val;
            }
        }
        cache_len = target;
    }

    return cache[target - 1]; /* a(n) is at index n-1 */
}
