/*
 * A034005  Successive digits of Catalan numbers.
 * https://oeis.org/A034005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 31, u32 n <= 93, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Successive digits of Catalan numbers
 * a(n) is the nth digit in the infinite word
 *   C(0)C(1)C(2)C(3)...   (concatenated decimal representations of Catalan numbers)
 *  C(k) = binomial(2k,k)/(k+1).
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/catalan.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

seq_t a(idx_t n)
{
    /* We look for the Catalan number that contains the requested digit. */
    idx_t pos = n;          /* target index within concatenated digits */
    idx_t cumulative = 0;   /* how many digits seen so far */
    idx_t k = 0;            /* current Catalan index */
    char buf[2000];         /* buffer large enough for cached Catalan numbers */
    size_t len;

    for (;;) {
        seq_t cat = catalan(k);             /* Catalan(k) */
        seq_to_str(cat, buf, &len);        /* convert to decimal string */
        if (cumulative + (idx_t)len > pos) {
            /* The desired digit lies in this Catalan number. */
            idx_t offset = pos - cumulative;   /* zero‑based offset within the string */
            return (seq_t)(buf[offset] - '0');
        }
        cumulative += (idx_t)len;
        ++k;
    }
}
