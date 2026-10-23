/*
 * A138487  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 3.
 * https://oeis.org/A138487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3.
   For n>0, let b(n) be the decimal digits of a(n-1) read from right to left.
   For each distinct digit d appearing in b(n) in the order of first appearance
   (i.e., from right to left), write the total number of occurrences of d
   followed by d, concatenated to form a(n).  This generates OEIS A138487. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"

/* Compute the next term of the sequence from the previous term. */
static seq_t compute_next(seq_t prev)
{
    int freq[10] = {0};
    int seen[10] = {0};
    int order[10] = {0};
    int ocnt = 0;
    seq_t t = prev;

    /* Extract digits from right to left, count frequencies and record order. */
    while (t > 0) {
        int d = (int)(t % 10);
        t /= 10;
        freq[d]++;
        if (!seen[d]) {
            order[ocnt++] = d;
            seen[d] = 1;
        }
    }

    /* Build the next term by concatenating count and digit for each recorded digit. */
    seq_t next = 0;
    for (int i = 0; i < ocnt; ++i) {
        int d = order[i];
        seq_t c = freq[d];

        /* Determine number of decimal digits in the count c. */
        int clen = 0;
        seq_t tmpc = c;
        do {
            ++clen;
            tmpc /= 10;
        } while (tmpc > 0);

        /* Append count and digit. */
        next = next * pow10((idx_t)clen) + c;
        next = next * 10 + d;
    }
    return next;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 3;
    }
    seq_t cur = 3;
    for (idx_t i = 1; i <= n; ++i) {
        cur = compute_next(cur);
    }
    return cur;
}
