/*
 * A027964  T(n,n+4), T given by A027960.
 * https://oeis.org/A027964
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 6*a(n-3) + a(n-4) - 3*a(n-5) +
 *            a(n-6)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A027964 : T(n,n+4) from A027960, offset 4 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;                       /* undefined for n < 4 */

    const size_t idx = (size_t)(n - 4);         /* zero‑based index in the sequence */

    /* initial six values: a(4) .. a(9) */
    if (idx <= 5) {
        static const seq_t init[6] = {
            (seq_t)1,
            (seq_t)7,
            (seq_t)26,
            (seq_t)73,
            (seq_t)174,
            (seq_t)373
        };
        return init[idx];
    }

    /* keep the last six values in a circular buffer */
    seq_t buf[6];
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)7;
    buf[2] = (seq_t)26;
    buf[3] = (seq_t)73;
    buf[4] = (seq_t)174;
    buf[5] = (seq_t)373;

    for (size_t cur = 6; cur <= idx; ++cur) {
        /* a(n-1)=buf[5], a(n-2)=buf[4], ... a(n-6)=buf[0] */
        seq_t pos = ((seq_t)5) * buf[5];
        pos += ((seq_t)6) * buf[3];
        pos += buf[2];
        pos += buf[0];

        seq_t neg = ((seq_t)9) * buf[4];
        neg += ((seq_t)3) * buf[1];

        seq_t next = pos - neg;                /* recurrence */

        /* shift window left by one */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];
}
