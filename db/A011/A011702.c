/*
 * A011702  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^7+x^5+x^4+x^3+x^2+1.
 * https://oeis.org/A011702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-7)+a(n-8) (mod 2) for n ≥
 *            8 Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 *            a(5)=0, a(6)=0, a(7)=1 This recurrence generates the binary
 *            m‑sequence that is the reciprocal of x^8+x^7+x^5+x^4+x^3+x^2+1
 *            modulo 2. The sequence is 85‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for the OEIS sequence A011702.  The result is returned
   as a seq_t value (0 or 1).  The function memoises previously computed
   values, so calls in arbitrary order run in O(n) time for each new term. */
seq_t a(idx_t n)
{
    static seq_t *buf = NULL;        /* cache of computed terms */
    static size_t cap  = 0;          /* capacity of buf */
    static idx_t  size = 0;          /* number of terms currently in buf */

    /* initial values for indices 0..7 */
    static const seq_t init8[8] =
        {0,0,0,0,0,0,0,1};

    /* enlarge the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* fatal failure */
        buf  = tmp;
        cap  = new_cap;
    }

    /* compute missing terms up to index n */
    for (idx_t i = size; i <= n; ++i) {
        seq_t val;
        if (i < 8) {
            val = init8[i];
        } else {
            /* recurrence: a(i) = a(i-2)^a(i-3)^a(i-4)^a(i-5)^a(i-7)^a(i-8) */
            val = buf[i-2] ^ buf[i-3] ^ buf[i-4] ^ buf[i-5] ^ buf[i-7] ^ buf[i-8];
        }
        buf[i] = val;
    }
    size = n + 1;
    return buf[n];
}
