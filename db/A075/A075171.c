/*
 * A075171  Nonnegative integers mapped to Dyck path encodings of the rooted
 *          plane trees obtained by recursing on the run lengths of the binary
 *          expansion of n.
 * https://oeis.org/A075171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by reading the binary representation of n. For
 *            each maximal run of identical bits of length L if L is odd: the
 *            run contributes the Dyck word 1 ( (10) repeated (L-1) times ) 0
 *            if L is even: the run contributes the word (L/2+1) times '1'
 *            followed by (L/2+1) times '0'. All produced...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline void append_run(seq_t *res, idx_t L)
{
    if (L == 0) return;           /* no run – never happens */
    if (L & 1) {                  /* odd length  -> star tree      */
        *res = (*res) * 10 + 1;   /* leading '1'                  */
        for (idx_t i = 0; i < L - 1; ++i) {
            *res = (*res) * 10 + 1;   /* '1' of the leaf            */
            *res = (*res) * 10 + 0;   /* '0' of the leaf            */
        }
        *res = (*res) * 10 + 0;   /* closing '0' of the root    */
    } else {                      /* even length -> chain tree   */
        idx_t m = L / 2 + 1;      /* number of nodes in the chain */
        for (idx_t i = 0; i < m; ++i) *res = (*res) * 10 + 1;
        for (idx_t i = 0; i < m; ++i) *res = (*res) * 10 + 0;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* find most significant set bit (msb position) */
    int msb = 0;
    for (int i = 63; i >= 0; --i)
        if ((n >> i) & 1) { msb = i; break; }

    seq_t result = 0;
    int prev_bit = (n >> msb) & 1;
    idx_t run_len = 1;

    for (int i = msb - 1; i >= 0; --i) {
        int bit = (n >> i) & 1;
        if (bit == prev_bit) {
            ++run_len;
        } else {
            append_run(&result, run_len);
            run_len = 1;
            prev_bit = bit;
        }
    }
    append_run(&result, run_len);   /* last run */
    return result;
}
