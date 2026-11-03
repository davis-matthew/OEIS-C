/*
 * A107988  Numbers of the form (4^i)*(11^j), with i, j >= 0.
 * https://oeis.org/A107988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest positive integer of the form 4^i * 11^j
 *            (i,j ≥ 0)
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 87, u64 n <= 321,
 *            u128 n <= 1234, big any n
 * Validated: standard, pass, 54 terms through n = 8865 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Lazily generated cache of the sequence */
    static seq_t *seq = NULL;          /* array storing the sequence values   */
    static idx_t size = 0;             /* number of values currently stored   */
    static idx_t cap  = 0;             /* allocated capacity                   */
    static idx_t ip4 = 0;              /* index used for the next 4‑multiple   */
    static idx_t ip11 = 0;             /* index used for the next 11‑multiple  */

    if (n == 0)            /* undefined for n < 1; return 0 for safety */
        return 0;

    /* Initial allocation: the first term is 1 (4^0 * 11^0) */
    if (seq == NULL) {
        cap  = 128;                  /* start with a modest capacity   */
        seq  = mem_new(seq_t, cap);
        seq[0] = 1;                  /* term a(1) = 1                  */
        size  = 1;
        ip4   = 0;
        ip11  = 0;
    }

    /* Extend the cache until we have at least n terms */
    while (size < n) {
        /* Grow the storage if necessary */
        if (size >= cap) {
            cap *= 2;
            seq = mem_resize(seq_t, seq, cap);
        }

        /* Candidate values generated from the two basic generators */
        seq_t next4  = seq[ip4]  * 4;   /* multiply by 4 */
        seq_t next11 = seq[ip11] * 11;  /* multiply by 11 */

        /* Choose the smaller candidate as the next sequence entry */
        seq_t next = (next4 < next11) ? next4 : next11;

        /* Advance the corresponding pointer(s) to avoid duplicates   */
        if (next == next4) ip4++;
        if (next == next11) ip11++;

        /* Append the chosen value to the cache */
        seq[size++] = next;
    }

    return seq[n - 1];                  /* a(1) is stored at index 0      */
}
