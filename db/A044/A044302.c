/*
 * A044302  Numbers k such that the string 5,6 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n‑th integer k such that the base‑9 representation of k
   contains the substring “56” whereas the base‑9 representation of k−1 does not. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t   *cache   = NULL;   /* cached sequence values   */
static size_t  cache_len = 0;     /* number of cached terms    */
static size_t  cache_cap = 0;     /* capacity of cache array   */

static idx_t  cur_k   = 1;        /* next integer to test     */
static bool   prev_has = false;   /* did cur_k−1 contain “56”?*/

seq_t a(idx_t n)
{
    if (n == 0)               /* safeguard for offset 1 */
        return 0;

    while (cache_len < n) {
        /* Detect whether cur_k contains the substring “56” in base‑9. */
        bool has = false;
        idx_t temp = cur_k;
        int prev_digit = -1;             /* more significant digit of previous loop */

        while (temp > 0) {
            int digit = (int)(temp % 9); /* current less significant digit   */
            if (digit == 5 && prev_digit == 6)  /* digits seen as …56 … */
                has = true;
            prev_digit = digit;
            temp /= 9;
        }

        if (has && !prev_has) {          /* k satisfies condition, previous does not */
            if (cache_len == cache_cap) { /* grow cache if necessary */
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_buf) {
                    exit(1);              /* allocation failure */
                }
                if (cache) {
                    memcpy(new_buf, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur_k;
        }

        prev_has = has;                   /* update state for next k            */
        ++cur_k;                          /* examine next integer                */
    }

    return cache[n - 1];
}
