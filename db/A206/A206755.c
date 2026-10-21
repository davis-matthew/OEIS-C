/*
 * A206755  Number of (n+1)X3 0..2 arrays with every 2X3 or 3X2 subblock
 *          having no more than three equal edges, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A206755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A206755
   Recurrence (for n>10):
       a(n) = 16*a(n-1) + 86*a(n-2) + 577*a(n-3) + 694*a(n-4) + 1376*a(n-5)
              - 823*a(n-6) + 136*a(n-7) - 2266*a(n-8) - 461*a(n-9) - 1230*a(n-10)
   Initial values:
       a(1)=106, a(2)=2187, a(3)=46860, a(4)=1002015, a(5)=21403411,
       a(6)=457325164, a(7)=9771502433, a(8)=208781853511,
       a(9)=4460929715062, a(10)=95314283208603
   (Values that do not fit into seq_t are truncated; the sequence is only
   required to be exact when it fits in the concrete seq_t type.) */

#include <stdlib.h>

/* The integer types are supplied by the build system; no definitions here. */

seq_t a(idx_t n)
{
    /* static cache: buf[1..sz] holds a(1)..a(sz) */
    static seq_t *buf = NULL;
    static size_t sz = 0;   /* current number of stored terms (excluding buf[0]) */
    static size_t cap = 0;  /* allocated capacity of buf */

    /* convert index to ordinary size_t for array indexing */
    size_t k = (size_t)n;

    /* trivial: below offset */
    if (k == 0)
        return (seq_t)0;

    /* initialise cache if it doesn't exist yet */
    if (!buf)
    {
        cap = 16;                     /* enough space for the initial 10 terms */
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf)
            return (seq_t)0;          /* allocation failed */
        buf[0] = (seq_t)0;            /* dummy slot, unused */
    }

    /* enlarge buffer if necessary */
    if (k > cap)
    {
        size_t new_cap = cap;
        while (new_cap < k)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* reallocation failed */
        buf = tmp;
        cap = new_cap;
    }

    /* fill in the base terms once */
    if (sz == 0)
    {
        buf[1] = (seq_t)106;
        buf[2] = (seq_t)2187;
        buf[3] = (seq_t)46860;
        buf[4] = (seq_t)1002015;
        buf[5] = (seq_t)21403411;
        buf[6] = (seq_t)457325164;
        buf[7] = (seq_t)9771502433;
        buf[8] = (seq_t)208781853511;
        buf[9] = (seq_t)4460929715062;
        buf[10] = (seq_t)95314283208603;
        sz = 10;
    }

    /* if we already have the value, just return it */
    if (k <= sz)
        return buf[k];

    /* compute new terms up to requested index */
    for (size_t i = sz + 1; i <= k; ++i)
    {
        seq_t v = (seq_t)0;

        /* a(i) = 16*a(i-1) + 86*a(i-2) + 577*a(i-3) + 694*a(i-4) + 1376*a(i-5)
                 - 823*a(i-6) + 136*a(i-7) - 2266*a(i-8) - 461*a(i-9) - 1230*a(i-10) */
        v += (seq_t)16  * buf[i - 1];
        v += (seq_t)86  * buf[i - 2];
        v += (seq_t)577 * buf[i - 3];
        v += (seq_t)694 * buf[i - 4];
        v += (seq_t)1376* buf[i - 5];
        v -= (seq_t)823 * buf[i - 6];
        v += (seq_t)136 * buf[i - 7];
        v -= (seq_t)2266* buf[i - 8];
        v -= (seq_t)461 * buf[i - 9];
        v -= (seq_t)1230* buf[i -10];

        buf[i] = v;
    }

    sz = k;
    return buf[k];
}
