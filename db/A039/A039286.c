/*
 * A039286  Numbers whose base-5 representation has the same nonzero number of
 *          0's and 4's.
 * https://oeis.org/A039286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer whose base‑5 representation contains the
 *            same nonzero number of 0's and 4's
 * Range:     u8 n <= 31, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

struct seq_state {
    idx_t  n;          /* target index */
    idx_t  counter;    /* how many terms found so far */
    seq_t  result;     /* value of the nth term when found */
    bool   found;      /* true when nth term has been found */
};

/* Recursively generate all length‑len numbers in increasing order
   and stop as soon as the nth term is reached. */
static void
gen_len(int pos, int len, int count0, int count4, seq_t val,
        struct seq_state *s)
{
    if (s->found)
        return;

    if (pos == len) {
        if (count0 == count4 && count0 > 0) {
            s->counter++;
            if (s->counter == s->n) {
                s->result = val;
                s->found  = true;
            }
        }
        return;
    }

    for (int d = (pos == 0 ? 1 : 0); d < 5; d++) {
        int ncount0 = count0 + (d == 0);
        int ncount4 = count4 + (d == 4);
        seq_t nval   = val * 5 + (seq_t)d;
        gen_len(pos + 1, len, ncount0, ncount4, nval, s);
        if (s->found)
            return;
    }
}

seq_t a(idx_t n)
{
    /* a(1) is the first term listed in the OEIS b‑file. */
    if (n == 0)
        return 0;          /* undefined for indices below the offset */

    struct seq_state s = { n, 0, 0, false };

    for (int len = 1; !s.found; len++)
        gen_len(0, len, 0, 0, (seq_t)0, &s);

    return s.result;
}
