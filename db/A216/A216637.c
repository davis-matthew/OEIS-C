/*
 * A216637  Kolakoski sequence by 3-words.
 * https://oeis.org/A216637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = label of the 3‑word starting at position (n‑1)·3 in the
 *            1‑2 Kolakoski sequence, where the words
 *            {112,121,122,211,212,221} are labeled 1,2,3,4,5,6 respectively
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Mapping of a 3‑bit pattern (1 if the symbol is 2, 0 otherwise)
   to the label of the corresponding 3‑word.  The index is the
   binary number b2b1b0, where b2 corresponds to the first symbol. */
static const unsigned char _label[8] = {0,1,2,3,4,5,6};

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; value for n=0 is irrelevant */
        return 0;

    /* We need the first 3·n symbols of the Kolakoski sequence. */
    idx_t len = n * 3;
    seq_t *s = (seq_t *)malloc(len * sizeof(seq_t));
    if (!s)
        return 0;          /* allocation failure – unlikely for tested sizes */

    /* initialise the first three terms of Kolakoski: (1,2,2) */
    if (len > 0) s[0] = 1;
    if (len > 1) s[1] = 2;
    if (len > 2) s[2] = 2;

    idx_t write    = 0;     /* next write position in s                */
    idx_t read     = 0;     /* index of current run length sample      */
    seq_t cur      = 1;     /* current symbol (1 or 2)                 */
    idx_t remaining = 0;    /* remaining symbols to write in current run */

    /* generate the Kolakoski sequence up to length len */
    while (write < len) {
        if (remaining == 0) {
            remaining = (idx_t)s[read];
            read++;
        }
        s[write++] = cur;
        remaining--;
        if (remaining == 0)
            cur = 3 - cur;   /* toggle between 1 and 2 */
    }

    /* determine the 3‑word starting at position (n‑1)·3 */
    idx_t pos = (n - 1) * 3;
    unsigned int bits = 0;
    bits |= (s[pos]     == 2 ? 1 : 0) << 2;
    bits |= (s[pos + 1] == 2 ? 1 : 0) << 1;
    bits |= (s[pos + 2] == 2 ? 1 : 0);

    /* map to label 1..6; _label[0] and _label[7] are never used. */
    seq_t result = _label[bits];

    free(s);
    return result;
}
