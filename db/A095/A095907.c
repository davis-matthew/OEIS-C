/*
 * A095907  Digits in the concatenation of strings formed from a previous
 *          string by substituting "01" for "0" and "011" for "1"
 *          simultaneously at each occurrence. Start with [0].
 * https://oeis.org/A095907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n belongs to the lower Wythoff sequence * (floor(φ ·
 *            m) for some m), otherwise a(n)=0 (upper Wythoff sequence). *
 *            This implementation builds the string by iteratively * replacing
 *            every 0 with “01” and every 1 with “011”, concatenating * each
 *            intermediate string, and returns the n‑th d...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* static storage is retained between calls, so the function
   works for any order of n without recomputing from scratch */
static uint8_t  buffer[200000];        /* concatenated result */
static idx_t   buf_len  = 0;           /* current length of buffer   */

static uint8_t  cur[200000];           /* current string S   */
static idx_t   cur_len  = 1;           /* length of cur      */

static uint8_t  nxt[200000];           /* temporary new string */

seq_t a(idx_t n)
{
    /* Grow buffer until it contains at least n digits */
    while (buf_len < n) {
        /* Append current string to the buffer */
        for (idx_t i = 0; i < cur_len; ++i)
            buffer[buf_len++] = cur[i];

        /* Produce the next string in nxt */
        idx_t pos = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            if (cur[i] == 0) {          /* replace 0 -> 01 */
                nxt[pos++] = 0;
                nxt[pos++] = 1;
            } else {                     /* replace 1 -> 011 */
                nxt[pos++] = 0;
                nxt[pos++] = 1;
                nxt[pos++] = 1;
            }
        }

        /* Prepare for next iteration */
        cur_len = pos;
        for (idx_t i = 0; i < cur_len; ++i)
            cur[i] = nxt[i];
    }

    /* Return the n‑th digit (1‑based indexing) */
    return (seq_t)buffer[n - 1];
}
