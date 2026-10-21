/*
 * A067494  Powers of 8 with initial digit 1.
 * https://oeis.org/A067494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067494 – Powers of 8 with initial decimal digit 1
 *
 *  a(n) is the n‑th integer of the form 8^k (k≥0) whose decimal
 *  representation starts with the digit 1.  The function generates
 *  powers of eight successively; for each power it keeps the largest
 *  power of ten that is not greater than the current value.  By
 *  dividing the current power by that power of ten the most
 *  significant decimal digit is obtained.  Whenever this digit is 1
 *  the current value is cached as the next term of the sequence.
 */
#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cache of found terms          */
    static size_t  cap   = 0;           /* capacity of cache            */
    static size_t  sz    = 0;           /* number of terms currently    */

    /* state of current power of 8 and division divisor            */
    static seq_t cur_val  = (seq_t)1;   /* 8^exp                        */
    static seq_t divisor = (seq_t)1;   /* 10^(number_of_digits-1)      */
    static idx_t  exp     = 0;         /* exponent of current power     */

    /* make sure the cache can hold at least n terms               */
    ensure_cache(n, &cache, &cap);

    /* generate powers until the required number of terms is stored */
    while (sz < n) {

        if (sz > 0) {                  /* compute next power of 8       */
            cur_val = cur_val * (seq_t)8;
            exp++;

            /* adjust divisor if the number of digits increased      */
            while (cur_val / divisor >= (seq_t)10) {
                divisor = divisor * (seq_t)10;
            }
        }

        /* obtain the most significant decimal digit of cur_val     */
        seq_t msd = cur_val / divisor;

        if (msd == (seq_t)1) {
            cache[sz++] = cur_val;      /* store term of the sequence   */
        }
    }

    return cache[n - 1];
}
