/*
 * A213497  Number of (w,x,y) with all terms in {0,...,n} and w = min(|w-x|,
 *          |x-y|).
 * https://oeis.org/A213497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A213497
   Recurrence: a(n) = 2*a(n-2) + a(n-3) - a(n-4) – 2*a(n-5) + a(n-7)
   Initial values: a(0)=1, a(1)=4, a(2)=10, a(3)=17, a(4)=29, a(5)=40, a(6)=57, a(7)=73
   This implementation generates the sequence in O(n) time using a sliding
   window of the last seven terms. */
seq_t a(idx_t n)
{
    /* first seven terms (indices 0..6) */
    static const seq_t init[7] = {
        (seq_t)1,  /* a(0) */
        (seq_t)4,  /* a(1) */
        (seq_t)10, /* a(2) */
        (seq_t)17, /* a(3) */
        (seq_t)29, /* a(4) */
        (seq_t)40, /* a(5) */
        (seq_t)57  /* a(6) */
    };

    /* for n < 7, return the hard‑coded value */
    if (n < 7)
        return init[(idx_t)n];

    /* sliding window holding a(m-6) .. a(m)
       last[0] = a(m-6), last[6] = a(m)            */
    seq_t last[7];
    for (idx_t i = 0; i < 7; ++i)
        last[i] = init[i];

    /* compute terms from a(7) upwards */
    for (idx_t m = 7; m <= n; ++m) {
        seq_t val = (seq_t)2 * last[5]          /* 2*a(m-2) */
                 +           last[4]           /*   +a(m-3) */
                 -           last[3]           /*   -a(m-4) */
                 - (seq_t)2 * last[2]          /*   -2*a(m-5) */
                 +           last[0];          /*   +a(m-7) */

        /* shift window: discard last[0],
           move elements left, and insert new value at the end */
        for (idx_t i = 0; i < 6; ++i)
            last[i] = last[i + 1];
        last[6] = val; /* now last[6] is a(m) */
    }

    return last[6]; /* a(n) */
}
