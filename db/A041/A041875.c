/*
 * A041875  Denominators of continued fraction convergents to sqrt(459).
 * https://oeis.org/A041875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 999700*a(n-10) - a(n-20) for n>19 with a(0)= 1, a(1)= 2,
 *            a(2)= 5, a(3)= 7, a(4)=33, a(5)=700, a(6)=2833, a(7)=3533,
 *            a(8)=9899, a(9)=23331, a(10)=989801, a(11)=2002933,
 *            a(12)=4995667, a(13)=6998600, a(14)=32990067, a(15)=699790007,
 *            a(16)=2832150095, a(17)=3531940102, a(18)=9896030299, a(19)...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static const seq_t init_vals[20] = {
    (seq_t)1,      (seq_t)2,      (seq_t)5,      (seq_t)7,
    (seq_t)33,     (seq_t)700,    (seq_t)2833,   (seq_t)3533,
    (seq_t)9899,   (seq_t)23331,  (seq_t)989801, (seq_t)2002933,
    (seq_t)4995667,(seq_t)6998600,(seq_t)32990067,(seq_t)699790007,
    (seq_t)2832150095,(seq_t)3531940102,(seq_t)9896030299,(seq_t)23324000700
};

static void ensure_cache(idx_t n)
{
    idx_t new_len = n + 1;
    if (cache_len >= new_len)
        return;

    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache)
        exit(1);       /* allocation failure */

    /* fill initial values up to min(new_len,20) */
    idx_t upto = new_len < 20 ? new_len : 20;
    for (idx_t i = 0; i < upto; ++i)
        new_cache[i] = init_vals[i];

    /* compute further terms if needed */
    if (new_len > 20) {
        for (idx_t i = 20; i < new_len; ++i)
            new_cache[i] = (seq_t)999700 * new_cache[i-10] - new_cache[i-20];
    }

    free(cache);
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
