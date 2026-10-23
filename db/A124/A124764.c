/*
 * A124764  Number of non-falls (levels or rises) for compositions in standard
 *          order.
 * https://oeis.org/A124764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) counts the number of non‑falling adjacent pairs (bi <=
 *            bi+1) in the composition that has standard order index n.
 *            Compositions are listed by increasing sum; within each sum the
 *            compositions are ordered lexicographically by decreasing first
 *            part, then second, etc. For index n>0: s = floor(log2...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0u) return 0;            /* empty composition */

    /* ---- find s = floor(log2(n)) + 1 ---- */
    idx_t msb = 0;                     /* index of most significant bit */
    idx_t tmp = n;
    while (tmp >>= 1) ++msb;
    idx_t s = msb + 1;                 /* number of parts in the composition */

    /* ---- local variables for counting ---- */
    seq_t a_val = 0;                   /* result to return */
    idx_t  last_pos = 0;               /* position of the last separator */
    seq_t  last_part = 0;              /* length of the previous part */

    /* ----- offset inside compositions of sum s ----- */
    idx_t offset = n - ((idx_t)1 << (s - 1));   /* 0‑based index within the block */

    /* ---- scan each possible separator position ---- */
    for (idx_t pos = 1; pos < s; ++pos) {
        idx_t bit_pos = (s - 1) - pos;           /* bit used for this separator */
        bool separator = ((offset >> bit_pos) & 1ULL) != 0;

        if (separator) {
            seq_t part = pos - last_pos;        /* current part length */
            if (last_part != 0 && last_part <= part) a_val++;
            last_part = part;
            last_pos = pos;
        }
    }

    /* ----- final part after the last separator ----- */
    seq_t part = s - last_pos;                  /* length of the last part */
    if (last_part != 0 && last_part <= part) a_val++;

    return a_val;
}
