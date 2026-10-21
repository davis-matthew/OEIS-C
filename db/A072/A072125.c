/*
 * A072125  Concatenation of first n entries (digits) of A007376 is not
 *          divisible by 3.
 * https://oeis.org/A072125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n‑th position k in the Champernowne constant such
 *            that the sum of the first k digits is NOT divisible by 3
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cache of generated sequence values */
static seq_t *cache = NULL;         /* array of seq_t values     */
static size_t cache_len = 0;        /* number of values stored   */
static size_t cache_cap = 0;        /* allocated capacity        */

/* state of Champernowne constant generation */
static idx_t cur_num = 1;           /* current integer to emit   */
static char digit_buf[32];          /* MSB‑first digits of cur_num */
static int  digit_len = 0;          /* length of digit_buf        */
static int  digit_idx = 0;          /* next digit to use         */

/* running sum of digits modulo 3 */
static uint8_t sum_mod = 0;

/* current position in Champernowne string (1‑based) */
static idx_t pos = 0;

/* grow the cache when needed */
static void grow_cache(void)
{
    size_t new_cap = cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* allocation failure – terminate the program */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* convert cur_num to its decimal digits and store in digit_buf */
static void set_current_digits(void)
{
    idx_t t = cur_num;
    int rev[32];
    int k = 0;
    /* at least one digit */
    do {
        rev[k++] = t % 10;
        t /= 10;
    } while (t);
    digit_len = k;
    for (int i = 0; i < k; ++i) {
        digit_buf[i] = rev[k - 1 - i]; /* MSB first */
    }
}

seq_t a(idx_t n)
{
    /* one‑time initialization of the cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            abort();
        }
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* fetch digits of current number if needed */
        if (digit_idx == 0) {
            set_current_digits();
        }

        /* consume next digit */
        int d = digit_buf[digit_idx];
        digit_idx++;
        if (digit_idx == digit_len) {
            /* move to next integer */
            cur_num++;
            digit_idx = 0;
        }

        /* update position and sum modulo 3 */
        pos++;
        sum_mod = (sum_mod + (uint8_t)d) % 3U;

        /* if prefix not divisible by 3, record the position */
        if (sum_mod != 0U) {
            if (cache_len == cache_cap) {
                grow_cache();
            }
            cache[cache_len++] = (seq_t)pos;
        }
    }

    /* return the (n-1)-th stored value (1‑based indexing) */
    return cache[n - 1];
}
